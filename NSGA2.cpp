#include "pch.h"
#include "Population.h"
#include "Individual.h"
#include"GeneticAlgorithm.h"
#include "ExperimentConfig.h"

#include <filesystem>

#define POPULATION_SIZE EXPERIMENT_POPULATION_SIZE
#define GENE_SIZE EXPERIMENT_GENE_SIZE
#define CROSSOVER_PROB 0.9
#define MUTATION_PROB (1 / GENE_SIZE)
#define pai 3.1415

typedef vector<Individual*> Front;

// Paper/result archives:
// - EP: external archive for the current run.
// - EP_study: temporary archive used by the preprocessing learning stage.
// - total_best: nondominated union collected from all repeated runs of a task.
vector<Chromosome>total_best;
vector<Chromosome>EP_study;
vector<Chromosome>EP;
extern double max_delay_time;         //最大延迟时间

bool descending(Individual*, Individual*);
void getData(string spec);
void get_peer_time(string spec);
void get_peer_time2(string spec);
void get_peer_time3(string spec);
void get_peer_time4(string spec);
void get_peer_srcdistance(string spec);
void get_peer_desdistance(string spec);
void get_peer_sd_distance(string spec);
void get_peer_sd2_distance(string spec);
void change_delay(int a);
void init_Matrix();
void init_MatrixC();
void init_Study(vector<Chromosome>& EP_study);
void preProcessing(vector<Chromosome>& EP_study);
void clean_chromosome(Chromosome& chrome);
void local_search_lunliu(Chromosome& chrome, int r_lamda[], int index, vector<Chromosome>& EPxx);
bool is_better(Chromosome& chrome_1, Chromosome& chrome_2);
bool is_equal(Chromosome& chrome1, Chromosome& chrome2);
int select_aim();
void init_global_f();
void init_curr_f();
void compute_IoI();
void compute_f(Chromosome& chrome);
void update_EP(Chromosome& new_chromosome, vector<Chromosome>& EPP);
void update_Last_End1();
void update_Last_End2();

namespace fs = std::filesystem;

enum class InputFileRole
{
	TaskData,
	Matrix
};

struct TaskInputSpec
{
	double stop_time = 0.0;
	double max_delay = EXPERIMENT_DEFAULT_MAX_DELAY;
	string data_file;
	string time_file1;
	string time_file2;
	string time_file3;
	string time_file4;
	string dis_file1;
	string dis_file2;
	string dis_file3;
	string dis_file4;
};

static string to_lower_ascii(string text)
{
	for (char& ch : text) {
		if (ch >= 'A' && ch <= 'Z') ch = char(ch - 'A' + 'a');
		if (ch == '\\') ch = '/';
	}
	return text;
}

static bool should_skip_search_dir(const fs::path& path)
{
	string name = to_lower_ascii(path.filename().string());
	return name == ".git" || name == ".vs" || name == "result" ||
		name == "stage" || name == "logs" || name == "runs" ||
		name == "debug" || name == "release" || name == "x64" ||
		name == "x86" || name == "__pycache__";
}

static int count_task_lines(const fs::path& path)
{
	ifstream fin(path);
	if (!fin.is_open()) return -1;
	int count = 0;
	string line;
	while (getline(fin, line)) {
		if (line.empty()) continue;
		if (line[0] == '#') continue;
		++count;
	}
	return count;
}

static vector<fs::path> collect_named_files(const fs::path& root, const string& filename)
{
	vector<fs::path> matches;
	if (root.empty() || !fs::exists(root)) return matches;

	try {
		fs::recursive_directory_iterator it(root, fs::directory_options::skip_permission_denied);
		fs::recursive_directory_iterator end;
		for (; it != end; ++it) {
			const fs::path current = it->path();
			if (it->is_directory()) {
				if (should_skip_search_dir(current)) it.disable_recursion_pending();
				continue;
			}
			if (it->is_regular_file() && current.filename().string() == filename) {
				matches.push_back(current);
			}
		}
	}
	catch (...) {
		// Dataset discovery should never stop the optimizer; callers will report
		// a clear missing-file error if no usable path is found.
	}
	return matches;
}

static fs::path locate_task_list()
{
	vector<fs::path> candidates;
	if (strlen(EXPERIMENT_TASK_LIST_PATH) > 0) {
		candidates.push_back(fs::path(EXPERIMENT_TASK_LIST_PATH));
	}

	candidates.push_back(fs::path("data") / "Mytxt.txt");
	candidates.push_back(fs::path("Mytxt.txt"));
	candidates.push_back(fs::path("data") / "realworld_45" / "data" / "Mytxt.txt");
	candidates.push_back(fs::path("data") / "solomon_36" / "data" / "Mytxt.txt");

	vector<fs::path> recursive_matches = collect_named_files(fs::current_path(), "Mytxt.txt");
	candidates.insert(candidates.end(), recursive_matches.begin(), recursive_matches.end());

	fs::path best_path;
	int best_score = INT_MAX;
	set<string> seen;
	for (const fs::path& candidate : candidates) {
		if (!fs::exists(candidate) || !fs::is_regular_file(candidate)) continue;
		string key = fs::absolute(candidate).lexically_normal().string();
		if (!seen.insert(key).second) continue;

		int lines = count_task_lines(candidate);
		if (lines <= 0) continue;

		int score = 10000;
		if (lines == EXPERIMENT_TASK_END) score -= 8000;
		else if (lines >= EXPERIMENT_TASK_END) score -= 4000;
		score += abs(lines - EXPERIMENT_TASK_END) * 10;

		string lowered = to_lower_ascii(candidate.generic_string());
		if (lowered == "data/mytxt.txt") score -= 1000;
		if (lowered.find("realworld_45") != string::npos && EXPERIMENT_TASK_END == 45) score -= 500;
		if (lowered.find("solomon_36") != string::npos && EXPERIMENT_TASK_END == 36) score -= 500;

		if (score < best_score) {
			best_score = score;
			best_path = candidate;
		}
	}
	return best_path;
}

static fs::path infer_task_root(const fs::path& task_list_path)
{
	fs::path list_dir = task_list_path.parent_path();
	if (list_dir.empty()) return fs::current_path();
	if (to_lower_ascii(list_dir.filename().string()) == "data") {
		fs::path parent = list_dir.parent_path();
		return parent.empty() ? fs::current_path() : parent;
	}
	return list_dir;
}

static int score_input_candidate(const fs::path& path, InputFileRole role)
{
	string lowered = to_lower_ascii(path.generic_string());
	int score = int(lowered.size());
	bool in_matrix_dir = lowered.find("time-distance-matrix") != string::npos;
	bool in_raw_dir = lowered.find("raw") != string::npos;

	if (role == InputFileRole::TaskData) {
		if (in_matrix_dir) score += 5000;
		if (in_raw_dir) score += 1000;
	}
	else {
		if (in_matrix_dir) score -= 1000;
		if (in_raw_dir) score += 5000;
	}
	return score;
}

static fs::path choose_best_input_candidate(vector<fs::path> candidates, InputFileRole role)
{
	fs::path best_path;
	int best_score = INT_MAX;
	set<string> seen;
	for (const fs::path& candidate : candidates) {
		if (!fs::exists(candidate) || !fs::is_regular_file(candidate)) continue;
		string key = fs::absolute(candidate).lexically_normal().string();
		if (!seen.insert(key).second) continue;
		int score = score_input_candidate(candidate, role);
		if (score < best_score) {
			best_score = score;
			best_path = candidate;
		}
	}
	return best_path;
}

static fs::path resolve_input_file(const string& token, const fs::path& task_root,
	const fs::path& task_list_dir, InputFileRole role)
{
	fs::path raw_path(token);
	vector<fs::path> candidates;

	if (raw_path.is_absolute()) candidates.push_back(raw_path);
	candidates.push_back(raw_path);
	candidates.push_back(task_list_dir / raw_path);
	candidates.push_back(task_root / raw_path);
	candidates.push_back(task_root / "data" / raw_path);
	candidates.push_back(task_root / "time-distance-matrix" / raw_path);

	fs::path direct = choose_best_input_candidate(candidates, role);
	if (!direct.empty()) return direct;

	string filename = raw_path.filename().string();
	vector<fs::path> recursive_candidates = collect_named_files(task_root, filename);
	return choose_best_input_candidate(recursive_candidates, role);
}

static bool parse_task_line(const string& line, TaskInputSpec& spec)
{
	if (line.empty() || line[0] == '#') return false;

	istringstream iss(line);
	vector<string> parts;
	string token;
	while (iss >> token) parts.push_back(token);

	if (parts.size() == 11) {
		spec.stop_time = atof(parts[0].c_str());
		spec.max_delay = atof(parts[1].c_str());
		spec.data_file = parts[2];
		spec.time_file1 = parts[3];
		spec.time_file2 = parts[4];
		spec.time_file3 = parts[5];
		spec.time_file4 = parts[6];
		spec.dis_file1 = parts[7];
		spec.dis_file2 = parts[8];
		spec.dis_file3 = parts[9];
		spec.dis_file4 = parts[10];
		return true;
	}

	if (parts.size() == 6) {
		spec.stop_time = atof(parts[0].c_str());
		spec.max_delay = EXPERIMENT_DEFAULT_MAX_DELAY;
		spec.data_file = parts[1];
		spec.time_file1 = parts[2];
		spec.time_file2 = parts[3];
		spec.time_file3 = parts[4];
		spec.time_file4 = parts[5];
		spec.dis_file1 = spec.time_file1;
		spec.dis_file2 = spec.time_file2;
		spec.dis_file3 = spec.time_file3;
		spec.dis_file4 = spec.time_file4;
		return true;
	}

	cout << "[InputError] Unsupported Mytxt line format with " << parts.size()
		<< " columns. Expected 6 or 11 columns." << endl;
	return false;
}

// Maintain the final task-level nondominated set. This is used after each run
// to merge the run-level EP into total_best before writing "final result".
bool update_best(Chromosome& new_chromosome, vector<Chromosome>& best) {
	int size = EP.size();
	if (best.size() == 0)best.push_back(new_chromosome);
	else {
		vector<Chromosome>::iterator iter = best.begin();
		for (; iter != best.end(); ++iter) {
			if (is_better(*iter, new_chromosome) || is_equal(*iter, new_chromosome)) return false;
		}
		iter = best.begin();
		while (iter != best.end()) {
			if (is_better(new_chromosome, *iter)) iter = best.erase(iter);
			else ++iter;
		}
		best.push_back(new_chromosome);
	}
	return true;
}


// Paper mechanism: IoP/SPO dynamic objective selection.
// At the beginning of each generation, the best value of each objective is
// compared with its historical best to compute IoP. A softmax over IoP gives
// SPO, which is then used to sample the target objective during evolution.
struct ObjectiveSelectionState
{
	vector<vector<double> > best_history;
	double historical_best[FUNC_NUM1];
	bool has_historical_best;
	double iop[FUNC_NUM1];
	double spo[FUNC_NUM1];

	ObjectiveSelectionState()
	{
		has_historical_best = false;
		for (int i = 0; i < FUNC_NUM1; ++i) {
			historical_best[i] = 1.0e100;
			iop[i] = 0.0;
			spo[i] = 1.0 / FUNC_NUM1;
		}
	}
};

const char* objective_mode_name()
{
	if (EXPERIMENT_OBJECTIVE_MODE == OBJECTIVE_ROUND_ROBIN) return "round_robin";
	if (EXPERIMENT_OBJECTIVE_MODE == OBJECTIVE_RANDOM) return "random";
	return "iop_spo";
}

void set_uniform_spo(ObjectiveSelectionState& state)
{
	for (int i = 0; i < FUNC_NUM1; ++i) {
		state.iop[i] = 0.0;
		state.spo[i] = 1.0 / FUNC_NUM1;
	}
}

// Called once per generation before offspring generation. All four objectives
// are minimization objectives, so the current generation best is min(f[m]).
void update_objective_selection_state(Population& parent, ObjectiveSelectionState& state)
{
	vector<double> best(FUNC_NUM1, 1.0e100);
	for (int i = 0; i < parent.individualSet.size(); ++i) {
		for (int m = 0; m < FUNC_NUM1; ++m) {
			if (parent.individualSet[i].genes[0].f[m] < best[m]) {
				best[m] = parent.individualSet[i].genes[0].f[m];
			}
		}
	}
	state.best_history.push_back(best);
	set_uniform_spo(state);

	if (!state.has_historical_best) {
		for (int m = 0; m < FUNC_NUM1; ++m) {
			state.historical_best[m] = best[m];
		}
		state.has_historical_best = true;
		return;
	}

	double max_iop = -1.0e100;
	for (int m = 0; m < FUNC_NUM1; ++m) {
		double current_best = best[m];
		double history_best = state.historical_best[m];
		double denominator = max(current_best, history_best) + EXPERIMENT_IOP_MU;
		if (fabs(denominator) < EXPERIMENT_IOP_MU) denominator = EXPERIMENT_IOP_MU;
		state.iop[m] = fabs(current_best - history_best) / denominator;
		if (state.iop[m] > max_iop) max_iop = state.iop[m];
	}

	double exp_sum = 0.0;
	for (int m = 0; m < FUNC_NUM1; ++m) {
		state.spo[m] = exp(state.iop[m] - max_iop);
		exp_sum += state.spo[m];
	}
	if (exp_sum <= 0.0) {
		set_uniform_spo(state);
		return;
	}
	for (int m = 0; m < FUNC_NUM1; ++m) {
		state.spo[m] /= exp_sum;
	}
	for (int m = 0; m < FUNC_NUM1; ++m) {
		if (best[m] < state.historical_best[m]) {
			state.historical_best[m] = best[m];
		}
	}
}

// Select the objective used by crossover, mutation, and tie-breaking. In the
// complete paper configuration this is not fixed round-robin; it is sampled
// from SPO so objectives with larger recent improvement potential receive more
// search effort.
int select_objective_by_config(ObjectiveSelectionState& state, int round_robin_index)
{
	if (EXPERIMENT_OBJECTIVE_MODE == OBJECTIVE_ROUND_ROBIN) {
		return round_robin_index % FUNC_NUM1;
	}
	if (EXPERIMENT_OBJECTIVE_MODE == OBJECTIVE_RANDOM) {
		return rand() % FUNC_NUM1;
	}

	double p = (rand() % 10000) / 10000.0;
	double cumulative = 0.0;
	for (int m = 0; m < FUNC_NUM1; ++m) {
		cumulative += state.spo[m];
		if (p <= cumulative) return m;
	}
	return FUNC_NUM1 - 1;
}

void log_objective_selection_state(ObjectiveSelectionState& state, int generation)
{
	if (!EXPERIMENT_ENABLE_LOG) return;
	cout << "[ObjectiveSelection] generation=" << generation
		 << " mode=" << objective_mode_name() << " IoP=";
	for (int m = 0; m < FUNC_NUM1; ++m) cout << state.iop[m] << (m == FUNC_NUM1 - 1 ? "" : ",");
	cout << " SPO=";
	for (int m = 0; m < FUNC_NUM1; ++m) cout << state.spo[m] << (m == FUNC_NUM1 - 1 ? "" : ",");
	cout << endl;
}

void ensure_output_dirs()
{
	_mkdir("Result");
	_mkdir("Stage");
}

void write_runtime_config()
{
	ensure_output_dirs();
	// Persist the exact paper/ablation switches beside the output files so each
	// run can be traced back to ExperimentConfig.h.
	ofstream cfg("Result\\run_config.txt", ios::out);
	cfg << "OBJECTIVE_MODE=" << objective_mode_name() << endl;
	cfg << "AOS_MODE=" << (EXPERIMENT_AOS_MODE == AOS_PM ? "PM" : (EXPERIMENT_AOS_MODE == AOS_AP ? "AP" : "MAB")) << endl;
	cfg << "CROSSOVER_MODE=" << (EXPERIMENT_CROSSOVER_MODE == CROSSOVER_ENCO ? "ENCO" : "DISABLED") << endl;
	cfg << "REWARD_MODE=" << (EXPERIMENT_REWARD_MODE == REWARD_IMPROVEMENT ? "IMPROVEMENT" : "FREQUENCY") << endl;
	cfg << "INIT_ROTATION_DIVERSITY=" << (EXPERIMENT_INIT_ROTATION_DIVERSITY ? "true" : "false") << endl;
	cfg << "POPULATION_SIZE=" << EXPERIMENT_POPULATION_SIZE << endl;
	cfg << "GENERATION_LIMIT=" << EXPERIMENT_GENERATION_LIMIT << endl;
	cfg << "RUN_COUNT=" << EXPERIMENT_RUN_COUNT << endl;
	cfg << "NOI_LIMIT=" << EXPERIMENT_NOI_LIMIT << endl;
	cfg << "TASK_START=" << EXPERIMENT_TASK_START << endl;
	cfg << "TASK_END=" << EXPERIMENT_TASK_END << endl;
	cfg << "IOP_WINDOW_H=" << EXPERIMENT_IOP_WINDOW_H << endl;
	cfg << "IOP_MU=" << EXPERIMENT_IOP_MU << endl;
	cfg << "ALPHA=" << EXPERIMENT_ALPHA << endl;
	cfg << "PM_PMIN=" << EXPERIMENT_PM_PMIN << endl;
	cfg << "AP_PMAX=" << EXPERIMENT_AP_PMAX << endl;
	cfg << "AP_BETA=" << EXPERIMENT_AP_BETA << endl;
	cfg << "MAB_C=" << EXPERIMENT_MAB_C << endl;
	cfg << "EPSILON_BOX=" << EXPERIMENT_EPSILON_BOX << endl;
	cfg << "ENABLE_PREPROCESSING=" << (EXPERIMENT_ENABLE_PREPROCESSING ? "true" : "false") << endl;
	cfg << "PREPROCESS_SECONDS=" << EXPERIMENT_PREPROCESS_SECONDS << endl;
	cfg << "USE_FIXED_SEED=" << (EXPERIMENT_USE_FIXED_SEED ? "true" : "false") << endl;
	cfg << "BASE_SEED=" << EXPERIMENT_BASE_SEED << endl;
	cfg << "ENABLE_LOG=" << (EXPERIMENT_ENABLE_LOG ? "true" : "false") << endl;
	cfg << "MUTATION_OPERATOR_ENABLED=";
	for (int i = 0; i < 8; ++i) {
		cfg << (EXPERIMENT_MUTATION_OPERATOR_ENABLED[i] ? "M" : "-") << (i + 1);
		if (i != 7) cfg << ",";
	}
	cfg << endl;
	cfg.close();
}
void output(vector<Chromosome>& EP, int generation, int task) {
	sort(EP.begin(), EP.end());
	// Remove duplicate/dominated archive members before writing one run result.
	vector<Chromosome> tru_EP;
	for (int a = 0; a < EP.size(); a++) {
		update_best(EP[a], tru_EP);
	}
	int i = 0, j = 0, size = tru_EP.size();
	FILE* pf;
	char name[40];
	sprintf_s(name, "Result\\task %d result %d .txt", task, generation);
	fopen_s(&pf, name, "w");
	for (i = 0; i < size; ++i) {
		for (j = 0; j < FUNC_NUM1; ++j) {
			fprintf(pf, "%lf  ", tru_EP[i].f[j]);
			printf("%lf ", tru_EP[i].f[j]);
		}
		printf("\n");
		fprintf(pf, "\n");
	}
	fclose(pf);
}

void output_best(int task) {
	FILE* pf;
	FILE* pf2;
	char name[80];
	char name2[80];
	sprintf_s(name, "Result\\task %d Finalresult.txt", task);
	fopen_s(&pf, name, "w");
	sprintf_s(name2, "Result\\task %d final result.txt", task);
	fopen_s(&pf2, name2, "w");
	sort(total_best.begin(), total_best.end());
	vector<Chromosome>::iterator iter;
	for (iter = total_best.begin(); iter != total_best.end(); ++iter) {
		printf("%lf %lf %lf %lf \n", iter->f[0], iter->f[1], iter->f[2], iter->f[3]);
		if (pf) fprintf(pf, "%lf %lf %lf %lf \n", iter->f[0], iter->f[1], iter->f[2], iter->f[3]);
		if (pf2) fprintf(pf2, "%lf %lf %lf %lf \n", iter->f[0], iter->f[1], iter->f[2], iter->f[3]);
	}
	//	printf("total solutions: %d\n", total_best.size());
	if (pf) fclose(pf);
	if (pf2) fclose(pf2);
}

void output_stage(vector<Chromosome>& EP, int generation, int task, int stage) {
	sort(EP.begin(), EP.end());
	// Stage snapshots use the same nondominated filtering as run outputs.
	vector<Chromosome> tru_EP;
	for (int a = 0; a < EP.size(); a++) {
		update_best(EP[a], tru_EP);
	}
	int i = 0, j = 0, size = tru_EP.size();
	FILE* pf;
	char name[40];
	sprintf_s(name, "Stage\\task %d result %d stage %d.txt", task, generation, stage);
	fopen_s(&pf, name, "w");
	for (i = 0; i < size; ++i) {
		for (j = 0; j < FUNC_NUM1; ++j) {
			fprintf(pf, "%lf  ", tru_EP[i].f[j]);
			printf("%lf ", tru_EP[i].f[j]);
		}
		printf("\n");
		fprintf(pf, "\n");
	}
	fclose(pf);
}

void process(double stop_time, int generation, int run, int w) {
	// Paper mechanism: learning-based preprocessing initializes operator
	// rewards before the evolutionary loop.
	init_global_f();
	init_Matrix();
	EP_study.clear();
	if (EXPERIMENT_ENABLE_PREPROCESSING) {
		preProcessing(EP_study);
	}
	update_Last_End1();
	if (EXPERIMENT_ENABLE_LOG) cout << "**************start****************" << endl;
	// Start the timed evolutionary search for one run.
	clock_t start, finish;
	GeneticAlgorithm ga;
	Population _tmpParent(POPULATION_SIZE, GENE_SIZE);
	Population _tmpChild(POPULATION_SIZE, GENE_SIZE);
	ObjectiveSelectionState objective_state;
	// Step 1: initialize and evaluate the parent population.
	_tmpParent.initialize();
	_tmpParent.evaluation();
	start = clock();
	int flag = -1;
	int stage = 1;
	/*
	Algorithm flow for each generation:
	1. Update IoP/SPO and select a target objective for each offspring attempt.
	2. Select two parents through Binary_tournament_selection(); note that the
	   current implementation samples one parent per call.
	3. Generate a child using enhanced greedy crossover when enabled.
	4. Apply AOS-selected local-search mutation.
	5. Keep the better solution and update the epsilon external archive.
	6. Run NSGA-II environmental selection on parent + offspring.
	*/
	while (1) {
		for (int i = 0; i < generation; i++) {
			init_curr_f();
			init_MatrixC();
			update_objective_selection_state(_tmpParent, objective_state);
			log_objective_selection_state(objective_state, i);
			int NOI = 0;		// Inner offspring-generation iteration count.
			// The enhanced crossover is used in the early search period; the
			// later period keeps mutation/local search as the main intensifier.
			finish = clock();
			if ((double)((finish - start) / CLOCKS_PER_SEC) < (0.75 * stop_time))
				flag = 1;
			else
				flag = 0;
			_tmpChild.clear();
			// Offspring generation module from Algorithm 1.
			while (NOI < EXPERIMENT_NOI_LIMIT) {	//50
				if (EXPERIMENT_ENABLE_LOG) cout << "NOI : " << NOI << endl;
				// Parent-selection entry point. The function name follows the
				// paper/BTS terminology, while the current code samples one parent.
				Chromosome Par1 = ga.Binary_tournament_selection(_tmpParent);
				Chromosome Par2 = ga.Binary_tournament_selection(_tmpParent);
				for (int aim_poll = 0; aim_poll < FUNC_NUM1; aim_poll++) {
					int aim = select_objective_by_config(objective_state, aim_poll);
					// Child after enhanced crossover or parent copy.
					Individual Child;
					Chromosome Child_chrome;
					Child.genes.push_back(Child_chrome);
					// Mutated child after AOS-selected local-search mutation.
					Individual Muta;
					Chromosome Muta_chrome;
					Muta.genes.push_back(Muta_chrome);
					// Winner between crossover child and mutation child.
					Individual X;
					Chromosome X_chrome;
					X.genes.push_back(X_chrome);
					bool use_crossover = (EXPERIMENT_CROSSOVER_MODE == CROSSOVER_ENCO && flag == 1);
					if (use_crossover) {
						if (!is_equal(Par1, Par2)) {
							// Use the better parent under the selected objective
							// as the leading parent for greedy crossover.
							if (Par1.f[aim] < Par2.f[aim]) {
								Child.genes[0] = ga.Crossover_Greedy(Par1, Par2, aim);
							}
							else {
								Child.genes[0] = ga.Crossover_Greedy(Par2, Par1, aim);
							}
						}
						else
							Child.genes[0] = Par1;
						Muta.genes[0] = ga.Mutation_Greedy(Child.genes[0], aim, EP);
					}
					else {
						if (Par1.f[aim] < Par2.f[aim]) {
							Child.genes[0] = Par1;
						}
						else {
							Child.genes[0] = Par2;
						}
						Muta.genes[0] = ga.Mutation_Greedy(Child.genes[0], aim, EP);
					}
					// Keep the better solution between crossover and mutation;
					// if nondomination cannot decide, use the selected objective.
					if (is_better(Child.genes[0], Muta.genes[0]))
						X.genes[0] = Child.genes[0];
					else if (is_better(Muta.genes[0], Child.genes[0]))
						X.genes[0] = Muta.genes[0];
					else {
						if (Child.genes[0].f[aim] < Muta.genes[0].f[aim]) {
							X.genes[0] = Child.genes[0];
						}
						else
							X.genes[0] = Muta.genes[0];
					}
					if (ga.Unique_population(_tmpParent, X.genes[0])) {
						_tmpChild.individualSet.push_back(X);
						update_EP(X.genes[0], EP);
					}
				}
				_tmpChild.evaluation();
				NOI++;
			}
			update_Last_End2();
			// NSGA-II environmental selection: nondominated sorting + crowding.
			Population _tmpComb = _tmpParent.combination(_tmpChild);
			vector<Front> front = ga.fastNonDominatedSort(&_tmpComb);
			_tmpParent.clear();
			int j = 0;
			while (_tmpParent.individualSet.size() + front[j].size() <= POPULATION_SIZE)
			{
				if (front[j].size() == 0) {
					//cin.get();
					if (EXPERIMENT_ENABLE_LOG) cout << "cin.get()" << endl;
					break;
				}
				ga.crowdingDistanceAssignment(front[j], -(pow(10.0, 3.0)), pow(10.0, 3.0));
				for (Individual* ind : front[j])
				{
					_tmpParent.individualSet.push_back(*ind);
				}
				j++;
			}
			sort(front[j].begin(), front[j].end(), descending);
			int size = _tmpParent.individualSet.size();
			for (int k = 0; k < (POPULATION_SIZE - size); k++)
			{
				_tmpParent.individualSet.push_back(*front[j][k]);
			}
			for (auto i : _tmpParent.individualSet)
			{
				update_EP(i.genes[0], EP);
			}
			if (EXPERIMENT_ENABLE_LOG) {
				std::cout << "Parent size: " << _tmpParent.individualSet.size() << endl;
				std::cout << "Archive size: " << EP.size() << endl;
				std::cout << "Complete generation [" << i << "]" << endl;
			}
			finish = clock();
			if ((double)((finish - start) / CLOCKS_PER_SEC) > ((stop_time * stage) / 25)) {
				output_stage(EP, run, w, stage);
				stage += 1;
			}
			if ((double)((finish - start) / CLOCKS_PER_SEC) > stop_time)
				break;
		}
		finish = clock();
		if ((double)((finish - start) / CLOCKS_PER_SEC) > stop_time)break;
	}
}


//int main()
//{
//	double stoptime;
//	string data_file;
//	string time_file1, time_file2, time_file3, time_file4;
//	string dis_file1, dis_file2, dis_file3, dis_file4;
//	string Myalltxt = "Mytxt.txt";
//	int front_task = 1;
//	int tail_task = 45;
//	fstream r_Myall(Myalltxt, ios::in);
//	char line[300];
//	for (int i = 1; i < front_task; ++i)
//	{
//		r_Myall.getline(line, 300);
//		std::cout << line << endl;
//	}
//	for (int i = front_task; i <= tail_task; i++)
//	{
//		srand(time(NULL));
//		clock_t start, finish;
//		std::cout << "=================start task " << i << "============================" << endl;
//		r_Myall >> stoptime;
//		r_Myall >> max_delay_time;
//		r_Myall >> data_file;
//		r_Myall >> time_file1;
//		r_Myall >> time_file2;
//		r_Myall >> time_file3;
//		r_Myall >> time_file4;
//		r_Myall >> dis_file1;
//		r_Myall >> dis_file2;
//		r_Myall >> dis_file3;
//		r_Myall >> dis_file4;
//
//		getData(data_file);
//		get_peer_time(time_file1);
//		get_peer_time2(time_file2);
//		get_peer_time3(time_file3);
//		get_peer_time4(time_file4);
//		get_peer_srcdistance(dis_file1);
//		get_peer_desdistance(dis_file2);
//		get_peer_sd_distance(dis_file3);
//		get_peer_sd2_distance(dis_file4);
//
//		start = clock();
//		for (int run = 0; run < 30; run++)
//		{
//			char out_file[100];
//			char num[3];
//			//取整数输入值，并将其转换为10进制数字的字符串
//			itoa(run, num, 10);
//
//			process(stoptime, 10);
//
//			printf("generation:%d\n", run);
//			output(EP, run, i);
//			for (int j = 0; j < EP.size(); j++) {
//				update_best(EP[j], total_best);
//			}
//			EP.clear();
//		}
//		finish = clock();
//		output_best(i);
//		total_best.clear();
//	}
//	r_Myall.close();
//	return 0;
//}

int main()
{

	ensure_output_dirs();
	write_runtime_config();

	double stoptime;
	int front_task = EXPERIMENT_TASK_START;
	int tail_task = EXPERIMENT_TASK_END;

	fs::path task_list_path = locate_task_list();
	if (task_list_path.empty()) {
		cout << "Cannot locate Mytxt.txt. Put the benchmark under a unified data directory, such as data/Mytxt.txt or data/<benchmark>/data/Mytxt.txt." << endl;
		return 1;
	}
	fs::path task_root = infer_task_root(task_list_path);
	fs::path task_list_dir = task_list_path.parent_path();
	cout << "[InputConfig] task_list=" << task_list_path.string() << endl;
	cout << "[InputConfig] task_root=" << task_root.string() << endl;

	fstream r_Myall(task_list_path, ios::in);
	if (!r_Myall.is_open()) {
		cout << "Cannot open task list: " << task_list_path.string() << endl;
		return 1;
	}

	string line;
	int task_index = 0;
	while (getline(r_Myall, line))
	{
		TaskInputSpec task_spec;
		if (!parse_task_line(line, task_spec)) continue;
		++task_index;
		if (task_index < front_task) continue;
		if (task_index > tail_task) break;

		clock_t start, finish;
		std::cout << "=================start task " << task_index << "============================" << endl;
		stoptime = task_spec.stop_time;
		max_delay_time = task_spec.max_delay;

		fs::path data_file = resolve_input_file(task_spec.data_file, task_root, task_list_dir, InputFileRole::TaskData);
		fs::path time_file1 = resolve_input_file(task_spec.time_file1, task_root, task_list_dir, InputFileRole::Matrix);
		fs::path time_file2 = resolve_input_file(task_spec.time_file2, task_root, task_list_dir, InputFileRole::Matrix);
		fs::path time_file3 = resolve_input_file(task_spec.time_file3, task_root, task_list_dir, InputFileRole::Matrix);
		fs::path time_file4 = resolve_input_file(task_spec.time_file4, task_root, task_list_dir, InputFileRole::Matrix);
		fs::path dis_file1 = resolve_input_file(task_spec.dis_file1, task_root, task_list_dir, InputFileRole::Matrix);
		fs::path dis_file2 = resolve_input_file(task_spec.dis_file2, task_root, task_list_dir, InputFileRole::Matrix);
		fs::path dis_file3 = resolve_input_file(task_spec.dis_file3, task_root, task_list_dir, InputFileRole::Matrix);
		fs::path dis_file4 = resolve_input_file(task_spec.dis_file4, task_root, task_list_dir, InputFileRole::Matrix);

		vector<pair<string, fs::path> > required_files = {
			{"data_file", data_file},
			{"time_file1", time_file1},
			{"time_file2", time_file2},
			{"time_file3", time_file3},
			{"time_file4", time_file4},
			{"dis_file1", dis_file1},
			{"dis_file2", dis_file2},
			{"dis_file3", dis_file3},
			{"dis_file4", dis_file4}
		};
		bool missing_file = false;
		for (const auto& item : required_files) {
			if (item.second.empty() || !fs::exists(item.second)) {
				cout << "[InputError] task=" << task_index << " missing " << item.first << endl;
				missing_file = true;
			}
		}
		if (missing_file) {
			cout << "[InputError] Please copy benchmark CSV files into the unified data layout described in data/README.md." << endl;
			return 1;
		}

		getData(data_file.string());
		get_peer_time(time_file1.string());
		get_peer_time2(time_file2.string());
		get_peer_time3(time_file3.string());
		get_peer_time4(time_file4.string());
		get_peer_srcdistance(dis_file1.string());
		get_peer_desdistance(dis_file2.string());
		get_peer_sd_distance(dis_file3.string());
		get_peer_sd2_distance(dis_file4.string());

		start = clock();
		for (int run = 0; run < EXPERIMENT_RUN_COUNT; run++)
		{
			unsigned int seed = EXPERIMENT_USE_FIXED_SEED
				? (EXPERIMENT_BASE_SEED + task_index * 1000 + run)
				: ((unsigned int)time(NULL) + task_index * 1000 + run);
			srand(seed);
			cout << "[RunConfig] task=" << task_index << " run=" << run << " seed=" << seed << endl;

			process(stoptime, EXPERIMENT_GENERATION_LIMIT, run, task_index);

			printf("generation:%d\n", run);
			output(EP, run, task_index);
			for (int j = 0; j < EP.size(); j++) {
				update_best(EP[j], total_best);
			}
			EP.clear();
			EP_study.clear();
		}
		finish = clock();
		output_best(task_index);
		total_best.clear();
	}
	r_Myall.close();
	return 0;
}

//ofstream os("NSGA2-solomon-5-10.txt");
//	if (os.is_open())
//	{
//		os << "NSGA2" << endl;
//		sort(total_best.begin(), total_best.end());
//		for (auto i : total_best)
//		{
//			os << i.f[0] << "\t" << i.f[1] << "\t" << i.f[2] << "\t" << float(1 / i.f[3]) * 100 << "%" << endl;
//		}
//	}
//	os.close();

bool descending(Individual* a, Individual* b)
{
	if ((a->rank < b->rank) || ((a->rank == b->rank) && (a->distance > b->distance)))
	{
		return true;
	}
	else
	{
		return false;
	}
}
