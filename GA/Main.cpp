/////////////////////////////////////////////////////////////////
//
// 유전 알고리즘 ( 문병로 교수님 ) : HW1
//
// 컴퓨터공학부
// ============
//
// 2003-21698 정우성
//
/////////////////////////////////////////////////////////////////

// GRAPH 와 CHECKING 용은 아래에서 해당하는 주석을 제거한다.
//#define GRAPH
#define CHECKING

#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <math.h>

using namespace std;

////////////////////////////////////////////////////////////////
// 함수 선언
////////////////////////////////////////////////////////////////

// IO
void load_problemset(char* filename);

// Selection
int select_random_chromosome_from_population();
int select_tournament_chromosome_from_population();

// Crossover
void xover_uniform(int src1, int src2, int target);

// Mutation
void mutation(int index);

// Replacement
void replacement();
void replace_chromosome(int pop_index,int gen_index);

// Convergence
double calculate_percentage();

// Initial Process
void make_environment(char* report_file, int partition_count,int population_count,int replace_count,double tournament_winner_p,double xover_uniform_p,double mutation_p,double replace_bad_p,int pre_cut_count, int cut_count, double mutation_reduce_r, double replace_bad_reduce_r, double converge_rate, int stop_count);
void make_chromosome_space();
void create_initial_chromosomes();

// Main Process
void Process_GA();

// information
double sd_of_chromosome(int index);
double sd_of_generation(int index);

// else
void save_best_chromosome(int index);
void make_summary();
double get_population_average();

// set

// debug:print
void print_problemset();
void print_generation(int index);
void print_chromosome(int index);

////////////////////////////////////////////////////////////////
// 변수 선언
////////////////////////////////////////////////////////////////

////////////////
/* parameters */
////////////////

//io
char* g_filename;				//open할 file의 이름 실행화일과 같은 디렉토리에 있어야 함

//general
int g_population;				//population 의 크기
int g_partition_count;			//partition 개수. 즉, 몇개의 그룹으로 분할하는지에 대한 정보
int g_replace_count;			// 한번에 얼마나 많이 대치될 것인가에 대한 정보

//selection
double g_tournament_winner_p;	// 토너먼트 파라미터 ( 0 ~ 1 )

//crossover
double g_xover_uniform_p;		// uniform crossover의 임계확률, 즉 첫번째 염색체로부터 유전자를 가져올 확률 ( 0 ~ 1 )

//mutation
double g_mutation_p;			// 전형적 변이의 임계값, 즉 각 유전자가 변이를 일으킬 확률 ( 0 ~ 1 )

//replace
double g_replace_bad_p;			// Fitness가 더 낮은 경우라도 replace할 확률 ( 0 ~ 1 )

//termination
int g_converge_factor;			// 종료조건을 감지하기 위해 population 에서 새롭게 만들어지는 집단에서 더 좋은해가 얻어지지 못하는 경우 카운트
int g_converge_count;			// g_converge_factor>g_replace_count*(1-g_replace_bad_p) 인 경우 1증가 시킨다. 그렇지 않으면 0 으로 만든다.
int g_pre_cut_count;			// generation 에서 더 나쁜 해가 만들어질때 cut 을 하기 위한 기준 count
int g_cut_count;				// g_converge_count 가 이 값을 넘게 되면 g_mutation_p 와 g_replace_bad_p 를 반으로 줄인다.
double g_mutation_reduce_r;		// cut 시 감소비율
double g_replace_bad_reduce_r;	// cut 시 감소비율
double g_converge_rate;
int g_stop_count;

//else
int g_k;						// generation#
int g_test_num;					// test#


////////////
/* config */
////////////

////////////////////////////
/* inner global variables */
////////////////////////////

int	g_problem_size;					// problem set 의 크기 ( 문제 data의 개수 ) : 자동으로 입력됨
double g_best_sd=9999;				// sd 최소값
vector<int> g_best_chromosome;		// 탐색중 sd를 최소화 하는 염색체

vector<double> pbset;				// problem set 을 위한 공간 ( input )
vector< vector<int> > chromosome;	// population에 속해있는 염색체를 위한 공간
vector< vector<int> > generation;	// replace 하기 위해 염색체를 임시적으로 저장하기 위한 공간
vector<int> chromosome_tag;			// replace 될 후보에 대한 정보 표시 1:대체후보 0:대체하지않음

vector<double> sd;					// population 에 있는 chromosome 들의 표준편차
vector<double> subsum;				// sd를 구하기 위한 임시 공간
vector<double> gen_sd;				// generation 에 있는 chromosome 들의 표준편자
vector<double> gen_subsum;			// gen_sd를 구하기 위한 임시 공간

// debug

////////////
// report //
////////////
char *g_report_file;
ofstream outfile;

#ifdef GRAPH
ofstream graphfile;	// ### graph ###
#endif

double g_global_best_sd=9999999;
double g_global_best_time=9999999;
int g_global_best_sd_test_num=g_test_num;
int g_global_best_time_test_num=g_test_num;

vector<double> g_global_sd;
vector<double> g_global_time;

int main(int argc ,char *argv[])
{

	if(argc!=17)
	{
		printf("Error:Wrong parameters\n");
		printf("parameters info: inputfile(add .\\) outfile(add .\\) partition_count population_count replace_count tournament_winner_p xover_uniform_p mutation_p replace_bad_p pre_cut_count cut_count mutation_reduce_r replace_bad_reduce_r converge_rate stop_count test_count\n");
		return 1;
	}
	
	load_problemset(argv[1]);

#ifdef GRAPH
	string gf_temp; // ### graph
	gf_temp=argv[2]; // ### graph
	gf_temp=gf_temp+".graph.xls"; // ### graph
#endif

	printf("****************** GA TEST ******************\n\n");
	printf("total test#:\t %s\n",argv[16]);
	printf("problem size:\t %d\n",g_problem_size);
	printf("inputfile:\t %s\n",argv[1]);
	printf("outfile:\t %s\n",argv[2]);
#ifdef GRAPH
	printf("graph data:\t %s\n",gf_temp.c_str());
#endif
	printf("partition_count:\t %s\n",argv[3]);
	printf("population_count:\t %s\n",argv[4]);
	printf("replace_count:\t\t %s\n",argv[5]);
	printf("tournament_winner_p:\t %s\n",argv[6]);
	printf("xover_uniform_p:\t %s\n",argv[7]);
	printf("mutation_p:\t\t %s\n",argv[8]);
	printf("replace_bad_p:\t\t %s\n",argv[9]);
	printf("pre_cut_count:\t\t %s\n",argv[10]);
	printf("cut_count:\t\t %s\n",argv[11]);
	printf("mutation_reduce_r:\t %s\n",argv[12]);
	printf("replace_bad_reduce_r:\t %s\n",argv[13]);
	printf("converge_rate:\t\t %s\n",argv[14]);
	printf("stop_count:\t\t %s\n\n",argv[15]);
	printf("*********************************************\n\n");

	printf("Push any key to continue...");getchar();
	printf("\n\n");

	outfile.open(argv[2]);	// open for report

#ifdef GRAPH
	graphfile.open(gf_temp.c_str());	// ### graph
#endif

	outfile << "****************** GA TEST ******************" << endl << endl;
	outfile << "total test#:\t " << argv[16] << endl;
	outfile << "problem size:\t " << g_problem_size << endl;
	outfile << "inputfile:\t " << argv[1] << endl;
	outfile << "outfile:\t " << argv[2] << endl;
#ifdef GRAPH
	outfile << "graphfile:\t " << gf_temp << endl;
#endif
	outfile << "partition_count:\t " << argv[3] << endl;
	outfile << "population_count:\t " << argv[4] << endl;
	outfile << "replace_count:\t\t " << argv[5] << endl;
	outfile << "tournament_winner_p:\t " << argv[6] << endl;
	outfile << "xover_uniform_p:\t " << argv[7] << endl;
	outfile << "mutation_p:\t\t " << argv[8] << endl;
	outfile << "replace_bad_p:\t\t " << argv[9] << endl;
	outfile << "pre_cut_count:\t\t " << argv[10] << endl;
	outfile << "cut_count:\t\t " << argv[11] << endl;
	outfile << "mutation_reduce_r:\t " << argv[12] << endl;
	outfile << "replace_bad_reduce_r:\t " << argv[13] << endl;
	outfile << "converge_rate:\t\t " << argv[14] << endl;
	outfile << "stop_count:\t\t " << argv[15] << endl;
	outfile << "*********************************************" << endl << endl;

	// for global result
	g_global_sd.resize(atoi(argv[16]));
	g_global_time.resize(atoi(argv[16]));

	for(int a=0;a<atoi(argv[16]);a++)
	{
		g_test_num=a+1;

#ifdef GRAPH				  	
		graphfile << endl << "**************************[TEST " << g_test_num << "]**************************" << endl << endl; // ### graph ###
		graphfile << "gen#\tsd\taverage\tsd percentage\tmutation_r\treplace_bad_r" << endl;	// ### graph ###
		graphfile << "------------------------------------------------------------" << endl << endl; // ### graph ###
#endif
		printf("\n=================[TEST %d]=================\n",g_test_num);
		make_environment(argv[2],atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),atof(argv[6]),atof(argv[7]),atof(argv[8]),atof(argv[9]),atoi(argv[10]),atoi(argv[11]),atof(argv[12]),atof(argv[13]),atof(argv[14]),atoi(argv[15]));
		Process_GA();
	}

	make_summary();

	return 0;
}

// 각 parameter 들에 대한 parameter 값을 지정한다.
//void initialize_parameter()
void make_environment(char* report_file, int partition_count,int population_count,int replace_count,double tournament_winner_p,double xover_uniform_p,double mutation_p,double replace_bad_p,int pre_cut_count, int cut_count, double mutation_reduce_r, double replace_bad_reduce_r, double converge_rate, int stop_count)
{
	g_best_sd=9999;				// sd 최소값

	//general
	g_partition_count=partition_count;		//partition 개수. 즉, 몇개의 그룹으로 분할하는지에 대한 정보
	g_population=population_count;			// population 크기
	g_replace_count=replace_count;			// 한번에 얼마나 많이 대치될 것인가에 대한 정보

	//selection
	g_tournament_winner_p=tournament_winner_p;	// 토너먼트 파라미터

	//crossover
	g_xover_uniform_p=xover_uniform_p;		// uniform crossover의 임계확률, 즉 첫번째 염색체로부터 유전자를 가져올 확률 ( 0 ~ 1 )

	//mutation
	g_mutation_p=mutation_p;			// 전형적 변이의 임계값, 즉 각 유전자가 변이를 일으킬 확률 ( 0 ~ 1 )

	//replace
	g_replace_bad_p=replace_bad_p;		// Fitness가 더 낮은 경우라도 replace할 확률 ( 0 ~ 1 )

	//termination						//	g_cut_count= 100/sqrt(g_problem_size)+1;
	g_pre_cut_count = pre_cut_count;
	g_cut_count = cut_count;
	g_mutation_reduce_r = mutation_reduce_r;		// cut 시 감소비율
	g_replace_bad_reduce_r = replace_bad_reduce_r;	// cut 시 감소비율
	g_converge_rate=converge_rate;
	g_stop_count=stop_count;

	//else

	//아래의 함수들은 위의 값들이 정해진후에 실행되어야 한다.

	make_chromosome_space();

	subsum.resize(partition_count);
	gen_subsum.resize(partition_count);

	create_initial_chromosomes();

	gen_sd.resize(replace_count);
	generation.resize(replace_count);

	for(int i=0;i<replace_count;i++)
		generation[i].resize(g_problem_size);
	
	g_best_chromosome.resize(g_problem_size);

	// report	
	g_report_file = report_file;

}

void Process_GA()
{
	int a,b,i,stop_index=1;
	double t_p; //percentage temp
	
	time_t   start, finish;

	time( &start );
	printf("\n##### START ##### %s\n\n",ctime(&start));

	for(g_k=0;g_k<10000000;g_k++)
	{
		if(g_converge_factor>g_pre_cut_count)
			g_converge_count++;
		else
			g_converge_count=0;

		if(g_converge_count>g_cut_count)
		{
			g_mutation_p*=g_mutation_reduce_r;   
			g_replace_bad_p*=g_replace_bad_reduce_r;
			g_converge_count=0;

#ifdef CHECKING
			//### checking ###//
			printf("[%d].%d <REDUCE> : mutation_p=%f , replace_bad_p=%f\n",g_test_num,g_k,g_mutation_p,g_replace_bad_p);
#endif
			if( (g_mutation_p<=0.000001) && (g_replace_bad_p<=0.000001) )
				break;
		}

		g_converge_factor=1;

		for(i=0;i<g_replace_count;i++) 
		{
			gen_sd[i]=-1;
		}

		for(i=0;i<g_replace_count;i++)
		{

			a=select_tournament_chromosome_from_population();
			b=select_tournament_chromosome_from_population();
			xover_uniform(a,b,i);
			mutation(i);
		}

		replacement();

		t_p = calculate_percentage();

#ifdef GRAPH
		// ### graph ### //
		graphfile << g_k << "\t" << g_best_sd << "\t" << get_population_average() << "\t" << t_p << "\t" << g_mutation_p << "\t" << g_replace_bad_p << endl;
#endif
		if(t_p>g_converge_rate)
		{
#ifdef CHECKING
			//### checking ###//
			printf("[%d].%d <OVER CONVERGE RATE>: stop index=%d\n",g_test_num,g_k,stop_index);
#endif
			if(stop_index++>g_stop_count) break;
		}
		else
		{
			stop_index=1;
		}
		
	}

	/////////////////////////
	// END
	/////////////////////////

	time(&finish);
	printf("\n##### CONVERGE ##### %s\n\n",ctime(&finish));

	double elapsed_time;
	elapsed_time=difftime(finish,start);
	
	/////////////////////////
	// Display
	/////////////////////////
	printf("Elapsed time: %f seconds\n", elapsed_time);
	printf("Total loop: %d generations\n\n", g_k);

	printf("Result : %f\n\n", g_best_sd);

	for(i=0;i<g_problem_size;i++)
		printf("%d",g_best_chromosome[i]);
	printf("\n");
	
	/////////////////////////
	// Report
	/////////////////////////

	outfile << "[TEST" << g_test_num << "]" << endl;
	outfile << "Elapsed time: " << elapsed_time << " seconds" << endl;
	outfile << "Total loop: " << g_k << " generations" << endl << endl;

	outfile << "Result : " << g_best_sd << endl << endl;

	for(i=0;i<g_problem_size;i++)
		outfile << g_best_chromosome[i];
	outfile << endl << endl << endl;

	/////////////////////////
	// Global result
	/////////////////////////

	g_global_sd[g_test_num-1]=g_best_sd;
	g_global_time[g_test_num-1]=elapsed_time;

	if(g_global_best_sd>g_best_sd)
	{
		g_global_best_sd=g_best_sd;
		printf("\n[%d] <<< BETTER RESULT! >>> : sd=%f\n",g_test_num,g_best_sd);
		g_global_best_sd_test_num=g_test_num;
	}

	if(g_global_best_time>elapsed_time)
	{
		g_global_best_time=elapsed_time;
		printf("\n[%d] <<< SHORTER TIME! >>> : time=%f\n",g_test_num,elapsed_time);
		g_global_best_time_test_num=g_test_num;
	}

}

double calculate_percentage()
{
	int cnt=0;

	for(int i=0;i<g_population;i++)
	{
		if(sd_of_chromosome(i)<=g_best_sd) cnt++;
	}

	return double(cnt)/g_population;

}

void replacement()
{
	int i,tag_count=0,j=0;

	for(i=0;i<g_population;i++)
		if(chromosome_tag[i]==1) tag_count++;

	while(tag_count-g_replace_count<0)
	{
		int worst_index=0;
		double worst_sd=-1;

		for(i=0;i<g_population;i++)
		{
			if((chromosome_tag[i]!=1) && (sd_of_chromosome(i)>worst_sd)) 
			{
				worst_index=i;
				worst_sd=sd_of_chromosome(i);
			}
		}
		if(worst_sd<0) break;
		chromosome_tag[worst_index]=1;
		tag_count++;
	}

	int startp,endp;

	startp=(int)(g_population*rand()/(RAND_MAX+1));
	endp = (startp+g_population-1) % g_population;

	i=startp;
	while(i++!=endp)
	{
		if(i>=g_population) i=0;
		if(chromosome_tag[i]==1)
		{
			replace_chromosome(i,j);
			chromosome_tag[i]=0;
			if(++j>=g_replace_count) return;
		}
	}

}

void replace_chromosome(int pop_index,int gen_index)
{

	if(sd_of_chromosome(pop_index)<=(sd_of_generation(gen_index)))  
	{
		g_converge_factor++;

		if ((double(rand())/RAND_MAX)>g_replace_bad_p) 
		{
			return;
		}

	}

	for(int i=0;i<g_problem_size;i++)
	{
		chromosome[pop_index][i]=generation[gen_index][i];
	}

	if(sd_of_generation(gen_index)<g_best_sd) 
	{
		g_best_sd=sd_of_generation(gen_index);
		save_best_chromosome(gen_index);
		printf("[%d].%d << BETTER CHROMOSOME >> : sd=%f\n",g_test_num,g_k,g_best_sd);
		//print_chromosome(gen_index);

	}

}
void save_best_chromosome(int index)
{
	for(int i=0;i<g_problem_size;i++)
	{
		g_best_chromosome[i]=generation[index][i];
	}
}

void xover_uniform(int src1, int src2, int target)
{
	for(int i=0;i<g_problem_size;i++)
	{
		if ((double(rand())/RAND_MAX)<g_xover_uniform_p) generation[target][i]=chromosome[src1][i];
		else generation[target][i]=chromosome[src2][i];
	}

	if(sd_of_generation(target)<sd_of_chromosome(src1))
		chromosome_tag[src1]=1;
	
	if(sd_of_generation(target)<sd_of_chromosome(src2))
		chromosome_tag[src2]=1;

}

void mutation(int index)
{
	for(int i=0;i<g_problem_size;i++)
		if ((double(rand())/RAND_MAX)<g_mutation_p) generation[index][i]=(int)(g_partition_count*rand()/(RAND_MAX+1));
}

double sd_of_chromosome(int index)
{
	int i;
	double avg=0;
	double sqr_sum=0;

	long double sd_temp=0;

	double temp=0;

//	if(sd[index]<0)
//	{

		for(i=0;i<g_partition_count;i++) subsum[i]=0;

		for(i=0;i<g_problem_size;i++) subsum[chromosome[index][i]]+=pbset[i];

		// 합들의 차
		if(g_partition_count<3)
		{
			sd_temp=subsum[1]-subsum[0];
			if(sd_temp<0) sd_temp=-sd_temp;
		}
		else // 표준편차
		{
			for(i=0;i<g_partition_count;i++) avg+=subsum[i];
			avg/=g_partition_count;

			sd_temp=0;
			for(i=0;i<g_partition_count;i++)
			{
				temp=subsum[i]-avg;
				sd_temp+=temp*temp;
			}
			sd_temp = sqrt(sd_temp/g_partition_count);

		}
//	}
		sd[index]=sd_temp;
		return sd_temp;
}

double sd_of_generation(int index)
{
	int i;
	double avg=0;
	double sqr_sum=0;

	long double sd_temp=0;

	double temp=0;

//	if(gen_sd[index]<0)
//	{

		for(i=0;i<g_partition_count;i++) subsum[i]=0;

		for(i=0;i<g_problem_size;i++) subsum[generation[index][i]]+=pbset[i];
		
		if(g_partition_count<3) // 합의 차
		{
			sd_temp=subsum[1]-subsum[0];
			if(sd_temp<0) sd_temp=-sd_temp;
		}
		else	//표준편차
		{
			for(i=0;i<g_partition_count;i++) avg+=subsum[i];
			avg/=g_partition_count;
			
			for(i=0;i<g_partition_count;i++)
			{
				temp=subsum[i]-avg;
				sd_temp+=temp*temp;
			}
			sd_temp = sqrt(sd_temp/g_partition_count);
		}
//	}
		gen_sd[index]=sd_temp;

	return sd_temp;
}

int select_random_chromosome_from_population()
{
	return int(g_population*rand()/(RAND_MAX+1));
}

int select_tournament_chromosome_from_population()
{
	int a,b,temp;
	double t;

	a=select_random_chromosome_from_population();
	b=select_random_chromosome_from_population();

	if(sd_of_chromosome(a)>sd_of_chromosome(b))
	{
		temp=a;
		a=b;
		b=temp;
	}
	
	//printf("D");
	chromosome_tag[b]=1;

	t=(double(rand())/RAND_MAX);

	if(t<g_tournament_winner_p) return a;
	else return b;
}

void create_initial_chromosomes()
{
	srand( (unsigned)time(NULL) );
	
	for(int i=0;i<g_population;i++)
	{
		for(int j=0;j<g_problem_size;j++)
		{
			chromosome[i][j]=(int)(g_partition_count*rand()/(RAND_MAX+1));
		}
	}
}

void make_chromosome_space()
{
	// chromosome 을 population 과 problemset size에 맞게 공간확보
	chromosome.resize(g_population);
	chromosome_tag.resize(g_population);
	sd.resize(g_population);

	for(int i=0;i<g_population;i++)
	{
		chromosome[i].resize(g_problem_size);
		chromosome_tag[i]=0;
		sd[i]=-1;
	}
}

void print_chromosome(int index)
{
	for(int j=0;j<g_problem_size;j++)
	{
		printf("%d",chromosome[index][j]);
	}
	printf("\n");
}

void print_generation(int index)
{
	for(int j=0;j<g_problem_size;j++)
	{
		printf("%d",generation[index][j]);
	}
	printf("\n");
}

void print_problemset()
{
	for(int i=0;i<g_problem_size;i++)
	{
		printf("%d, %f\n",i,pbset[i]);
	}
	printf("\n");
}

void load_problemset(char* filename)
{
	int problem_max_size=100;	// problem set 을 담기 위한 최대 크기 ( 이값보다 크게 되면 동적으로 2배증가 )

	g_problem_size=0;
	g_filename=filename;

	pbset.resize(problem_max_size);

	ifstream infile(g_filename);

	string s;
	while(getline(infile,s))
	{
		pbset[g_problem_size++]=atof(s.c_str());
		if(g_problem_size>problem_max_size-1)
			pbset.resize(problem_max_size<<=1);
	}

}

void make_summary()
{
	double sum_sd=0,sum_time=0;

	////////////////////////////////////
	// Display
	////////////////////////////////////
	printf("\n\ntest#\tsd\t\ttime\n");
	printf("------------------------------------------------\n");

	for(int i=0;i<g_test_num;i++)
	{
		sum_sd+=g_global_sd[i];
		sum_time+=g_global_time[i];
		printf("%d\t%f\t%f\n",i+1,g_global_sd[i],g_global_time[i]);
	}

	printf("\n\n------------------------------------------------\n");
	printf(    "Best result   : %f ( test# %d)\n",g_global_best_sd, g_global_best_sd_test_num);
	printf(    "Shortest time : %f ( test# %d)\n\n",g_global_best_time, g_global_best_time_test_num);

	printf(    "Average result: %f\n",sum_sd/g_test_num);
	printf(    "Average time  : %f\n",sum_time/g_test_num);

	////////////////////////////////////
	// Report
	////////////////////////////////////
	outfile << "\n\ntest#\tsd\t\ttime\n";
	outfile << "------------------------------------------------\n";
	
	sum_sd=0;
	sum_time=0;
		
	for(i=0;i<g_test_num;i++)
	{
		sum_sd+=g_global_sd[i];
		sum_time+=g_global_time[i];
		outfile << i+1 << "\t" << g_global_sd[i] << "\t" << g_global_time[i] << endl;
	}

	outfile << "\n\n------------------------------------------------\n";
	outfile << "Best result   : " << g_global_best_sd << " ( test# " << g_global_best_sd_test_num << ")" << endl;
	outfile << "Shortest time : " << g_global_best_time << " ( test# " << g_global_best_time_test_num << ")\n\n";

	outfile << "Average result: " << sum_sd/g_test_num << endl;
	outfile << "Average time  : " << sum_time/g_test_num << endl;

}

double get_population_average()
{
	double sum=0;

	for(int i=0;i<g_population;i++)
		sum+=sd_of_chromosome(i);

	return sum/g_population;
}