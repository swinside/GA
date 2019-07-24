//////////////////////////////////////////////////////////
//
// 유전 알고리즘 ( 문병로 교수님 ) HW2
//
// 2003-21698 정우성
// 2003-21739 현승훈
//
//////////////////////////////////////////////////////////

// 추후 : 다이나믹 프로그래밍 기법으로 속도향상가능, 중복계산을 피하고 결과를 메모리에 보관하여 변경이 없는 경우는 그냥 이용한다.
// 종료조건 개선 : best fitness 의 비율이 0 이 되는 경우가 있으므로 이 경우에도 종료할 수 있도록 해야한다.

//#define GRAPH
#define CHECKING
//#define PRINTPROCESS
#define NOENTER
#define USEDONEFILE

#define MAX_X 42			// 지수귀문도 최대 높이 MAX_X*2+1
#define MAX_Y 20			// 지수귀문도 가운데 6각형 나란히 이어진 것의 최대 개수
#define MAX_HEXA 1000		// 최대 육각형 수 < MAX_X * MAX_Y
#define MAX_POPULATION 1000	// chromosome 최대 개수 ( population 크기 )
#define MAX_LENGTH	1000	// chromosome 최대 길이 ( 지수귀문도 크기 )
#define MAX_TEST_NUM 1000
#define MAX_XOVER_POSITION 10

#include <fstream>
#include <time.h>
#include <math.h>

using namespace std;

// parameters
int g_min_width;			// 지수귀문도에서 이어진 평행 육각형들의 시작 길이 ( 가장 짧다 )
int g_max_width;			// 지수귀문도에서 이어진 평행 육각형들의 가운데 길이 ( 가장 길다 )
int g_population_size;		// population 크기
int g_generation_size;		// generation 크기
double g_tournament_winner_p;	// tournament_selection 에서 winner 가 선택될 확률
double g_xover_uniform_p;	// xover시에 첫번째 유전자로부터 상속받을 확률
double g_replace_bad_p;			// Fitness가 더 낮은 경우라도 replace할 확률 ( 0 ~ 1 )
double g_mutation1_p;
double g_mutation2_p;
int g_local_optimize_count;
int g_spin_count;
double g_do_deep_better_optimize_p;
double g_do_deep_worse_optimize_p;
double g_do_optimize_p;
double g_do_mutation1_p;
double g_do_mutation2_p;
short g_force_mutation2;
double g_do_spin_p;
double g_do_spin_apply_p;
double g_do_hexa_optimize_p;
int g_hexa_optimize_count;
int	g_hexa_optimize_inner_count;
int g_test_count;
double g_change_xover_src_p;
int g_xover_kind;
int g_xover_npt;
double g_do_xover_p;

double g_converge_rate_p;
int g_converge_count;

int g_opt_diff;

// data
int chromosome_index[MAX_X][MAX_Y][6];		// chromosome index 정보
int chromosome[MAX_POPULATION][MAX_LENGTH];	// 실제 chromosome 이 저장되는 곳
int chromosome_tag[MAX_POPULATION];			// replacement 를 위한 tag 정보
int i_to_x[MAX_HEXA];
int i_to_y[MAX_HEXA];
int xy_to_i[MAX_X][MAX_Y];
int g_best_chromosome[MAX_LENGTH];
int g_sol_chromosome[MAX_LENGTH];

int generation[MAX_POPULATION][MAX_LENGTH];	// generation 이 저장되는 곳

double g_global_fitness[MAX_TEST_NUM];
int g_global_solution_sum[MAX_TEST_NUM];
double g_global_time[MAX_TEST_NUM];

// else
int g_length;				// 지수귀문도에 들어가는 숫자개수
int g_hexa_count;			// 6각형개수
int g_test_num;
bool g_found;
double g_found_fitness;
char g_outputfile[80];
char g_graphfile[80];

// variable
long g_gen;
double g_best_fitness;

double g_global_best_fitness;
int g_global_best_fitness_test_num;
double g_global_best_time;
int g_global_best_time_test_num;
int g_global_best_solution_sum;
int g_global_best_solution_sum_test_num;

// Functions
bool is_valid_xy(int x, int y);						// xy좌표 유효성 검증
bool set_chromosome_index(int x,int y,int i,int p);	// chromosome index 정보 set ( 새로운 값으로 등록되면 true 반환, 그외 false )
int get_chromosome_index(int x, int y, int i);		// chromosome index 정보 get
void make_initial_population();						// 초기 population 생성
double fitness_of_chromosome(int index);			// index번째 chromosome 의 fitness
int select_random_chromosome_from_population();
int select_tournament_chromosome_from_population();

void xover_uniform(int src1, int src2, int target);
void xover_1pt(int src1,int src2,int target);
void xover_rand_pt(int src1,int src2,int target);
void xover_n_pt(int src1,int src2,int target,int npt);
void xover_no(int src1, int src2, int target);

void replacement();
void replace_chromosome(int pop_index,int gen_index);
void save_best_chromosome(int index);
void mutation1(int index);
bool spin(int index);
bool local_optimize(int index);
void mutation2(int index);
bool hexa_optimize(int index);
double calculate_percentage();
void init_global();
void initialize_environment();
void processGA();
void display_test_info();
void make_summary();
double get_population_average();

//debug
void print_chromosome(int i);
void print_generation(int i);

// graph
#ifdef GRAPH
ofstream graphfile;	// ### graph ###
#endif

//report
ofstream reportfile;

//donefile
ofstream donefile;
ofstream donefile2;

void display_test_info()
{
	printf("****************** GA TEST *******************\n\n");
	printf("output file:\t\t\t %s\n",g_outputfile);
	printf("total test#:\t\t\t %d\n",g_test_count);
	printf("hexa min width:\t\t\t %d\n",g_min_width);
	printf("hexa max width:\t\t\t %d\n",g_max_width);
	printf("hexa count:\t\t\t %d\n",g_hexa_count);
	printf("number count:\t\t\t %d\n",g_length);
	printf("population size:\t\t %d\n",g_population_size);
	printf("generation size:\t\t %d\n\n",g_generation_size);
	printf("tournament winner p:\t\t %f\n",g_tournament_winner_p);
	printf("do xover p:\t\t\t %f\n",g_do_xover_p);

	switch(g_xover_kind)
	{
	case 0:
		printf("xover:\t\t\t\t no crossover\n");
		break;
	case 1:
		printf("xover:\t\t\t\t 1 point\n");
		break;
	case 2:
		printf("xover:\t\t\t\t %d points\n",g_xover_npt);
		break;
	case 3:
		printf("xover:\trand points (change p)\t %f\n",g_change_xover_src_p);
		break;
	case 4:
		printf("xover:\tuniform\t\t%f\n",g_xover_uniform_p);
		break;
	default:
		printf("xover:\t\t\t\t no crossover\n");
	}
	printf("do mutation1 p:\t\t\t %f\n",g_do_mutation1_p);
	printf("mutation1 p:\t\t\t %f\n",g_mutation1_p);
	printf("do optimize p:\t\t\t %f\n",g_do_optimize_p);
	printf("local optimize count:\t\t %d\n",g_local_optimize_count);
	printf("do deep better optimize p:\t %f\n",g_do_deep_better_optimize_p);
	printf("do deep worse optimize p:\t %f\n",g_do_deep_worse_optimize_p);
	printf("do spin p:\t\t\t %f\n",g_do_spin_p);
	printf("spin count:\t\t\t %d\n",g_spin_count);
	printf("do spin apply p:\t\t %f\n",g_do_spin_apply_p);
	printf("do mutation2 p:\t\t\t %f\n",g_do_mutation2_p);
	printf("mutation2 p:\t\t\t %f\n",g_mutation2_p);
	printf("force mutation2:\t\t %d\n",g_force_mutation2);
	printf("replace bad p:\t\t\t %f\n\n",g_replace_bad_p);
	printf("converge rate p:\t\t %f\n",g_converge_rate_p);
	printf("converge count:\t\t\t %d\n",g_converge_count);


	//report
	reportfile << "****************** GA TEST *******************" << endl << endl;
	reportfile << "total test#:\t\t\t " << g_test_count << endl;
	reportfile << "hexa min width:\t\t\t " << g_min_width << endl;
	reportfile << "hexa max width:\t\t\t " << g_max_width << endl;
	reportfile << "hexa count:\t\t\t " << g_hexa_count << endl;
	reportfile << "number count:\t\t\t " << g_length << endl;
	reportfile << "population size:\t\t " << g_population_size << endl;
	reportfile << "generation size:\t\t " << g_generation_size << endl << endl;
	reportfile << "tournament winner p:\t\t " << g_tournament_winner_p << endl;
	reportfile << "do xover p:\t\t\t " << g_do_xover_p << endl;
	
	switch(g_xover_kind)
	{
	case 0:
		reportfile << "xover:\t\t\t\t no crossover" << endl;
		break;
	case 1:
		reportfile << "xover:\t\t\t\t 1 point" << endl;
		break;
	case 2:
		reportfile << "xover:\t\t\t\t " << g_xover_npt << "points" << endl;
		break;
	case 3:
		reportfile << "xover:\trand points (change p)\t " << g_change_xover_src_p << endl;
		break;
	case 4:
		reportfile << "xover:\tuniform\t\t" << g_xover_uniform_p << endl;
		break;
	default:
		reportfile << "xover:\t\t\t\t no crossover" << endl;
	}
	reportfile << "do mutation1 p:\t\t\t " << g_do_mutation1_p << endl;
	reportfile << "mutation1 p:\t\t\t " << g_mutation1_p << endl;
	reportfile << "do optimize p:\t\t\t " << g_do_optimize_p << endl;
	reportfile << "local optimize count:\t\t " << g_local_optimize_count << endl;
	reportfile << "do deep better optimize p:\t " << g_do_deep_better_optimize_p << endl;
	reportfile << "do deep worse optimize p:\t " << g_do_deep_worse_optimize_p << endl;
	reportfile << "do spin p:\t\t\t " << g_do_spin_p << endl;
	reportfile << "spin count:\t\t\t " << g_spin_count << endl;
	reportfile << "do spin apply p:\t\t " << g_do_spin_apply_p << endl;
	reportfile << "do mutation2 p:\t\t\t " << g_do_mutation2_p << endl;
	reportfile << "mutation2 p:\t\t\t " << g_mutation2_p << endl;
	reportfile << "force mutation2:\t\t " << g_force_mutation2 << endl;
	reportfile << "replace bad p:\t\t\t " << g_replace_bad_p << endl << endl;
	reportfile << "converge rate p:\t\t " << g_converge_rate_p << endl;
	reportfile << "converge count:\t\t\t " << g_converge_count << endl;

#ifdef GRAPH
	printf("\ngraph data:\t %s\n\n",g_reportfile);
#endif

	printf("\n**********************************************\n\n");
	reportfile << endl << "**********************************************" << endl << endl;

#ifndef NOENTER
	printf("Push [Enter] to continue...");getchar();
	printf("\n\n");
#endif
	
}

void init_global()
{
	g_opt_diff=2;

#ifdef GRAPH
	graphfile.open(g_graphfile);	// ### graph
#endif
	reportfile.open(g_outputfile);

	// global result
	g_global_best_fitness=-999999;
	g_global_best_fitness_test_num=0;
	g_global_best_solution_sum=0;
	g_global_best_solution_sum_test_num=0;
	g_global_best_time=999999;
	g_global_best_time_test_num=0;

	for(int i=0;i<MAX_TEST_NUM;i++)
		g_global_solution_sum[i]=0;

	// test parameter
//	g_xover_kind=3;//0 no / 1 1pt / 2 npt/ 3 randpt/ 4 uniform	/
//	g_xover_npt=4;
//	g_change_xover_src_p=0.16;

	/*
//	g_outputfile=".\\output.txt";
//	g_graphfile=".\\graph.xls";

	g_min_width=3;			// 지수귀문도에서 이어진 평행 육각형들의 시작 길이 ( 가장 짧다 )
	g_max_width=5;			// 지수귀문도에서 이어진 평행 육각형들의 가운데 길이 ( 가장 길다 )
	
	g_population_size=50;		// population 크기
	g_generation_size=12;		// 또는 10 // generation 크기
	
	g_tournament_winner_p=1;	//0.9도 괜찮음
	g_xover_uniform_p=0.697;
	
	g_replace_bad_p=0.195;
	
	g_do_optimize_p=1;
	g_local_optimize_count=6000;
	g_do_deep_better_optimize_p=1;
	g_do_deep_worse_optimize_p=0;
	
	g_do_spin_p=0;
	g_spin_count=1000;
	g_do_spin_apply_p=1;

    g_do_mutation1_p=0.1;
	g_mutation1_p=0.05;
	
	g_do_mutation2_p=0.12;
	g_mutation2_p=0.08;
	g_force_mutation2=0;

	//종료조건
	g_converge_rate_p=0.99;
	g_converge_count=5;

	//테스트 시행회수
	g_test_count=100;

*/

	/* 2,3 BEST 85찾음 , 1,2 ; 12,12 등도 유효
	g_outputfile=".\\output.txt";
	g_graphfile=".\\graph.xls";

	g_min_width=2;			// 지수귀문도에서 이어진 평행 육각형들의 시작 길이 ( 가장 짧다 )
	g_max_width=3;			// 지수귀문도에서 이어진 평행 육각형들의 가운데 길이 ( 가장 길다 )
	
	g_population_size=100;		// population 크기
	g_generation_size=30;		// 또는 10 // generation 크기
	
	g_tournament_winner_p=0.6;	//0.9도 괜찮음
	g_xover_uniform_p=0.6;
	
	g_replace_bad_p=0.1;
	
	g_do_optimize_p=1;
	g_local_optimize_count=300;
	g_do_deep_better_optimize_p=1;
	g_do_deep_worse_optimize_p=0;

    g_do_spin_p=0;
	g_spin_count=1000;
	g_do_spin_apply_p=1;

	g_do_mutation1_p=0.1;
	g_mutation1_p=0.05;
	
	g_do_mutation2_p=0.1;
	g_mutation2_p=0.1;
	g_force_mutation2=0;


	//종료조건
	g_converge_rate_p=0.99;
	g_converge_count=5;

	//테스트 시행회수
	g_test_count=100;


	/* 3,5 190찾았음
	g_outputfile=".\\output.txt";
	g_graphfile=".\\graph.xls";

	g_min_width=3;			// 지수귀문도에서 이어진 평행 육각형들의 시작 길이 ( 가장 짧다 )
	g_max_width=5;			// 지수귀문도에서 이어진 평행 육각형들의 가운데 길이 ( 가장 길다 )
	
	g_population_size=50;		// population 크기
	g_generation_size=12;		// 또는 10 // generation 크기
	
	g_tournament_winner_p=1;	//0.9도 괜찮음
	g_xover_uniform_p=0.697;
	
	g_replace_bad_p=0.195;
	
	g_do_optimize_p=1;
	g_local_optimize_count=6000;
	g_do_deep_better_optimize_p=1;
	g_do_deep_worse_optimize_p=0;
	
	g_do_spin_p=0;
	g_spin_count=1000;
	g_do_spin_apply_p=1;

    g_do_mutation1_p=0.1;
	g_mutation1_p=0.05;
	
	g_do_mutation2_p=0.12;
	g_mutation2_p=0.08;
	g_force_mutation2=0;

	//종료조건
	g_converge_rate_p=0.99;
	g_converge_count=5;

	//테스트 시행회수
	g_test_count=100;
	*/


}
void initialize_environment()
{
	int x,y,i,p=0,t_index=0;

	// 초기화
	for(x=0;x<(g_max_width-g_min_width)*2+1;x++)
		for(y=0;y<g_max_width;y++)
		{
			xy_to_i[x][y]=-1;
			for(i=0;i<6;i++)
				chromosome_index[x][y][i]=-1;
		}

	// 인덱싱 
	for(x=0;x<(g_max_width-g_min_width)*2+1;x++)
		for(y=0;y<g_max_width;y++)
			for(i=0;i<6;i++)
				if(set_chromosome_index(x,y,i,p))
				{
					p++;

					if(xy_to_i[x][y]<0)
					{
						xy_to_i[x][y]=t_index;
						i_to_x[t_index]=x;
						i_to_y[t_index]=y;
						t_index++;
					}
				}

	// 초기화
	g_length=p;
	g_hexa_count = t_index;
	g_best_fitness = -999999;
	g_found_fitness = -999999;
	g_found=false;

	if(t_index!=g_max_width + (g_max_width-g_min_width)*(g_max_width+g_min_width-1))
	{
		printf("error:calculation error - wrong hexa_count!\n");
		reportfile << "error:calculation error - wrong hexa_count!" << endl;
		return;
	}

}
void processGA()
{
	int i,j,a,b,stop_index=0,t_sum;
	bool t_success;
	double t_p;

	time_t   start, finish;
	time( &start );

	printf("\n##### START ##### %s\n\n",ctime(&start));
	reportfile << endl << "##### START ##### " << ctime(&start) << endl << endl;

	for(g_gen=1;g_gen<100000000;g_gen++)
	{	
		for(i=0;i<g_generation_size;i++)
		{
			a=select_tournament_chromosome_from_population();
			b=select_tournament_chromosome_from_population();

			if((double(rand())/RAND_MAX)<=g_do_xover_p)
			{
				switch(g_xover_kind)
				{
				case 0:
					xover_no(a,b,i);
					break;
				case 1:
					xover_1pt(a,b,i);
					break;
				case 2:
					xover_n_pt(a,b,i,g_xover_npt);
					break;
				case 3:
					xover_rand_pt(a,b,i);
					break;
				case 4:
					xover_uniform(a,b,i);
					break;
				default:
					xover_no(a,b,i);
				}
			}
			else
			{
				xover_no(a,b,i);
			}

			t_success=false;
			if((double(rand())/RAND_MAX)<=g_do_mutation1_p) mutation1(i);
			if((double(rand())/RAND_MAX)<=g_do_optimize_p) t_success=local_optimize(i);
			if((double(rand())/RAND_MAX)<=g_do_spin_p) t_success=(spin(i) | t_success);
			if((g_force_mutation2==1) || (!t_success) )
				if((double(rand())/RAND_MAX)<=g_do_mutation2_p) mutation2(i);		
		}
		replacement();

		t_p = calculate_percentage();

#ifdef CHECKING
		printf("[%d].%d %f (%f)\n",g_test_num,g_gen,-g_best_fitness,t_p);
		reportfile << "[" << g_test_num << "]." << g_gen << " " << -g_best_fitness << " (" << t_p << ")" << endl;
#endif

#ifdef GRAPH
		graphfile << g_gen << "\t" << -g_best_fitness << "\t" << get_population_average() << "\t" << t_p << endl;
#endif
		if(g_best_fitness==0) break;
		if(t_p>=g_converge_rate_p)
		{
			stop_index++;
#ifdef CHECKING
			printf("[%d].%d <OVER CONVERGE RATE>: stop index=%d\n",g_test_num,g_gen,stop_index);
			reportfile << "[" << g_test_num << "]." << g_gen << " <OVER CONVERGE RATE>: stop index=" << stop_index << endl;
#endif
			if(stop_index>=g_converge_count) break;
		}
		else
		{
			stop_index=0;
		}
	}
	/////////////////////////
	// END
	/////////////////////////

	time(&finish);

	printf("\n##### CONVERGE ##### %s\n\n",ctime(&finish));
	reportfile << endl << "##### CONVERGE ##### " << ctime(&finish) << endl; 

	double elapsed_time;
	elapsed_time=difftime(finish,start);
	
	/////////////////////////
	// Display
	/////////////////////////
	printf("Elapsed time: %f seconds\n", elapsed_time);
	printf("Total loop: %d generations\n\n", g_gen);

	reportfile << "Elapsed time: " << elapsed_time << " seconds" << endl;
	reportfile << "Total loop: " << g_gen << " generations" << endl << endl;

	//Result
	char *t_s="";

	if ((g_found==true) && (g_found_fitness==0))
		t_s="(Solution) ";
	else
		t_s="";

	printf("Result %s: %f\n\n", t_s,-g_best_fitness);
	reportfile << "Result " << t_s << ": " << -g_best_fitness << endl << endl;
	
	for(i=0;i<g_length;i++)
	{
		printf("%d ",g_best_chromosome[i]);
		reportfile << g_best_chromosome[i] << " ";
	}

	printf("\n\n");
	reportfile << endl << endl;

	for(i=0;i<g_hexa_count;i++)
	{
		t_sum=0;
		for(j=0;j<6;j++)
		{
			printf(" %d",g_best_chromosome[chromosome_index[i_to_x[i]][i_to_y[i]][j]]);
			reportfile << " " << g_best_chromosome[chromosome_index[i_to_x[i]][i_to_y[i]][j]];
			
			t_sum+=g_best_chromosome[chromosome_index[i_to_x[i]][i_to_y[i]][j]];
		}
			printf("\t => %d\n",t_sum);
			reportfile << "\t => " << t_sum << endl;
	}
	printf("\n");
	reportfile << endl;
	
	//Solution/
//	if((g_found == true) && (g_found_fitness==0))
//	{
	//	printf("Best solution sum : %f\n\n",t_sum);
		
	//	for(i=0;i<g_length;i++)
//		{
//			printf("%d ",g_sol_chromosome[i]);
//			reportfile << g_sol_chromosome[i] << " ";
//		}
//		printf("\n\n");
//		reportfile << endl << endl;

//		for(i=0;i<g_hexa_count;i++)
//		{
//			t_sum=0;
//			for(j=0;j<6;j++)
//			{
//				printf(" %d",g_sol_chromosome[chromosome_index[i_to_x[i]][i_to_y[i]][j]]);
//				reportfile << " " << g_sol_chromosome[chromosome_index[i_to_x[i]][i_to_y[i]][j]];
//
//				t_sum+=g_sol_chromosome[chromosome_index[i_to_x[i]][i_to_y[i]][j]];
//			}
//				printf("\t => %d\n",t_sum);
//				reportfile << "\t => " << t_sum << endl;
//		}
//		printf("\n");
///		reportfile << endl;
//	}


	/////////////////////////
	// Global result
	/////////////////////////

	g_global_fitness[g_test_num-1]=g_best_fitness;
	if(g_found==true) g_global_solution_sum[g_test_num-1]=t_sum;
	g_global_time[g_test_num-1]=elapsed_time;

	if(g_global_best_fitness<g_best_fitness)
	{
		g_global_best_fitness=g_best_fitness;
		g_global_best_fitness_test_num=g_test_num;
	}
	if((g_found==true) && (g_global_best_solution_sum<t_sum))
	{
		g_global_best_solution_sum=t_sum;
		g_global_best_solution_sum_test_num=g_test_num;
	}

	if(g_global_best_time>elapsed_time)
	{
		g_global_best_time=elapsed_time;
		g_global_best_time_test_num=g_test_num;
	}

}

int main(int argc ,char *argv[])
{
	if(argc==25)
	{
		//이전버전 
		// test parameter
		g_xover_kind=4;//0 no / 1 1pt / 2 npt/ 3 randpt/ 4 uniform	
		g_xover_npt=4;
		g_change_xover_src_p=0.16;
		g_do_xover_p=1;
	}
	else if(argc==29)
	{
		g_xover_kind=atoi(argv[25]); //0 no / 1 1pt / 2 npt/ 3 randpt/ 4 uniform	
		g_xover_npt=atoi(argv[26]);
		g_change_xover_src_p=atof(argv[27]);
		g_do_xover_p = atof(argv[28]);
	}
	else
	{
		printf("Error:Wrong parameters\n");
		return 1;
	}
	/////////////////////////////////
	// parameter setting
	/////////////////////////////////

	strcpy(g_outputfile,argv[1]);
	strcpy(g_graphfile,argv[2]);

	g_min_width=atoi(argv[3]);			// 지수귀문도에서 이어진 평행 육각형들의 시작 길이 ( 가장 짧다 )
	g_max_width=atoi(argv[4]);			// 지수귀문도에서 이어진 평행 육각형들의 가운데 길이 ( 가장 길다 )
	
	g_population_size=atoi(argv[5]);		// population 크기
	g_generation_size=atoi(argv[6]);		// 또는 10 // generation 크기
	
	g_tournament_winner_p=atof(argv[7]);	//0.9도 괜찮음
	g_xover_uniform_p=atof(argv[8]);
	
	g_replace_bad_p=atof(argv[9]);
	
	g_do_optimize_p=atof(argv[10]);
	g_local_optimize_count=atoi(argv[11]);
	g_do_deep_better_optimize_p=atof(argv[12]);
	g_do_deep_worse_optimize_p=atof(argv[13]);
	
	g_do_spin_p=atof(argv[14]);
	g_spin_count=atoi(argv[15]);
	g_do_spin_apply_p=atof(argv[16]);

    g_do_mutation1_p=atof(argv[17]);
	g_mutation1_p=atof(argv[18]);
	
	g_do_mutation2_p=atof(argv[19]);
	g_mutation2_p=atof(argv[20]);
	g_force_mutation2=atoi(argv[21]);

	//종료조건
	g_converge_rate_p=atof(argv[22]);
	g_converge_count=atoi(argv[23]);

	//테스트 시행회수
	g_test_count=atoi(argv[24]);

	init_global();
	initialize_environment();	
	display_test_info();

	for(g_test_num=1;g_test_num<g_test_count+1;g_test_num++)
	{
		printf("\n===================[TEST %d]===================\n",g_test_num);
		reportfile << endl << "===================[TEST " << g_test_num << "]===================" << endl;

#ifdef GRAPH				  	
		graphfile << endl << "*****************[TEST " << g_test_num << "]********************" << endl << endl; // ### graph ###
		graphfile << "gen#\tbest fitness\taverage fitness\tpercentage" << endl;	// ### graph ###
		graphfile << "----------------------------------------" << endl << endl; // ### graph ###
#endif

		initialize_environment();	
		make_initial_population();
		processGA();
	}
	
	make_summary();

	return 0;
}

int select_random_chromosome_from_population()
{
	return int(g_population_size*rand()/(RAND_MAX+1));
}

int select_tournament_chromosome_from_population()
{
	int a,b,temp;
	double t;

	a=select_random_chromosome_from_population();
	b=select_random_chromosome_from_population();

	if(fitness_of_chromosome(a)<fitness_of_chromosome(b))
	{
		temp=a;
		a=b;
		b=temp;
	}
	
	chromosome_tag[b]=1;

	t=(double(rand())/RAND_MAX);

	if(t<=g_tournament_winner_p) return a;
	else return b;
}

bool is_valid_xy(int x, int y)
{
	return
	(	
		(
		 ( x>=0 && x<g_max_width-g_min_width ) 
			&& 
		 ( y>=0 && y<x+g_min_width )
        )
		||
		( 
		 ( x>=g_max_width-g_min_width && x<(g_max_width-g_min_width)*2+1)
			&&
		 ( y>=x-(g_max_width-g_min_width) && y<g_max_width )
	    )
	);
}
bool set_chromosome_index(int x,int y,int i,int p)
{
	int p_temp;
	bool is_new=true;

	if(!is_valid_xy(x,y))
		return false;

	switch(i)
	{
	case 0:
		p_temp=get_chromosome_index(x-1,y-1,4);
		if(p_temp>0) 
		{
			p=p_temp;
			is_new=false;
		}
		p_temp=get_chromosome_index(x-1,y,2);
		if(p_temp>0)
		{
			p=p_temp;
			is_new=false;
		}
		chromosome_index[x][y][i]=p;
		break;

	case 1:
		p_temp=get_chromosome_index(x,y-1,5);
		if(p_temp>0) 
		{
			p=p_temp;
			is_new=false;
		}
		p_temp=get_chromosome_index(x-1,y-1,3);
		if(p_temp>0)
		{
			p=p_temp;
			is_new=false;
		}
		chromosome_index[x][y][i]=p;
		break;

	case 2:
		p_temp=get_chromosome_index(x,y-1,4);
		if(p_temp>0) 
		{
			p=p_temp;
			is_new=false;
		}
		p_temp=get_chromosome_index(x+1,y,0);
		if(p_temp>0)
		{
			p=p_temp;
			is_new=false;
		}
		chromosome_index[x][y][i]=p;
		break;

	case 3:
		p_temp=get_chromosome_index(x+1,y,5);
		if(p_temp>0) 
		{
			p=p_temp;
			is_new=false;
		}
		p_temp=get_chromosome_index(x+1,y+1,1);
		if(p_temp>0)
		{
			p=p_temp;
			is_new=false;
		}
		chromosome_index[x][y][i]=p;
		break;

	case 4:
		p_temp=get_chromosome_index(x+1,y+1,0);
		if(p_temp>0) 
		{
			p=p_temp;
			is_new=false;
		}
		p_temp=get_chromosome_index(x,y+1,2);
		if(p_temp>0)
		{
			p=p_temp;
			is_new=false;
		}
		chromosome_index[x][y][i]=p;
		break;

	case 5:
		p_temp=get_chromosome_index(x,y+1,1);
		if(p_temp>0) 
		{
			p=p_temp;
			is_new=false;
		}
		p_temp=get_chromosome_index(x-1,y,3);
		if(p_temp>0)
		{
			p=p_temp;
			is_new=false;
		}
		chromosome_index[x][y][i]=p;
		break;

	}

	return is_new;
}

int get_chromosome_index(int x, int y, int i)
{
	if(is_valid_xy(x,y))
	{
		return chromosome_index[x][y][i];
	}
	else return -1;
}

void make_initial_population()
{
	srand( (unsigned)time(NULL) );

	int i,j,t,temp[MAX_LENGTH];

	for(i=0;i<g_population_size;i++)
	{
		chromosome_tag[i]=0;
		
		for(j=0;j<g_length;j++) temp[j]=0;
		
		for(j=0;j<g_length;j++)
		{
			t=(int)(g_length*rand()/(RAND_MAX+1));
			while(temp[t]!=0) t=(t+1)%g_length;
			chromosome[i][j]=t+1;
			temp[t]=1;
		}
	}
}

double fitness_of_chromosome(int index)
{
	double temp=0;
	int i,j;
	double subsum[MAX_HEXA];
	double avg_subsum=0;
	
	for(i=0;i<g_hexa_count;i++)
	{
		subsum[i]=0;
		for(j=0;j<6;j++)
			subsum[i]+=chromosome[index][chromosome_index[i_to_x[i]][i_to_y[i]][j]];

		avg_subsum+=subsum[i];

	}

	avg_subsum/=g_hexa_count;

	for(i=0;i<g_hexa_count;i++)
		temp+=(subsum[i]-avg_subsum)*(subsum[i]-avg_subsum)/g_hexa_count;

//	return (avg_subsum-sqrt(temp)*sqrt(g_length));
	return (-sqrt(temp));

}

double fitness_of_generation(int index)
{
	double temp=0;
	int i,j;
	double subsum[MAX_HEXA];
	double avg_subsum=0;
	
	for(i=0;i<g_hexa_count;i++)
	{
		subsum[i]=0;
		for(j=0;j<6;j++)
			subsum[i]+=generation[index][chromosome_index[i_to_x[i]][i_to_y[i]][j]];
		avg_subsum+=subsum[i];

	}

	avg_subsum/=g_hexa_count;

	for(i=0;i<g_hexa_count;i++)
		temp+=(subsum[i]-avg_subsum)*(subsum[i]-avg_subsum)/g_hexa_count;

//	return (avg_subsum-sqrt(temp)*sqrt(g_length));
	return (-sqrt(temp));
}

void xover_rand_pt(int src1,int src2,int target)
{
	int i,j,src_pointer=0,is_used[MAX_LENGTH],t;
	int s1,s2,st;

	for(j=0;j<g_length;j++) is_used[j]=0;

	s1=src1;
	s2=src2;

	for(i=0;i<g_length;i++)
	{
		if ((double(rand())/RAND_MAX)<=g_change_xover_src_p) 
		{
			st=s1;
			s1=s2;
			s2=st;
		}

		j=0;
		t=chromosome[s1][i];
		if(is_used[t-1]) t=chromosome[s2][i];
		while (is_used[t-1]) t=chromosome[s1][j++];
		generation[target][i]=t;
		is_used[t-1]=1;
	}
}

void xover_no(int src1,int src2,int target)
{
	int i;

	for(i=0;i<g_length;i++)
	{
		generation[target][i]=chromosome[src1][i];
	}
}

void xover_n_pt(int src1,int src2,int target,int npt)
{
	int xover_position[MAX_XOVER_POSITION];
	int t,i,j,is_used[MAX_LENGTH];
	int s1,s2,st;

	for(j=0;j<g_length;j++) is_used[j]=0;

	for(j=0;j<npt;j++)
	{
		xover_position[j]= (int)(g_length*rand()/(RAND_MAX+1));
	}
	
	s1=src1;
	s2=src2;

	for(i=0;i<g_length;i++)
	{
		for(j=0;j<npt;j++)
		{
			if(xover_position[j]==i)
			{
				st=s1;
				s1=s2;
				s2=st;
				break;
			}
		}

		j=0;
		t=chromosome[s1][i];
		if(is_used[t-1]) t=chromosome[s2][i];
		while (is_used[t-1]) t=chromosome[s1][j++];
		generation[target][i]=t;
		is_used[t-1]=1;
	}
}

void xover_1pt(int src1,int src2,int target)
{
	int pos1,i,is_used[MAX_LENGTH],t,j;

	for(j=0;j<g_length;j++) is_used[j]=0;

	pos1 = (int)(g_length*rand()/(RAND_MAX+1));

	for(i=0;i<pos1;i++)
	{
		t=chromosome[src1][i];
		generation[target][i]=t;
		is_used[t-1]=1;
	}

	for(i=pos1;i<g_length;i++)
	{
		j=0;
		t=chromosome[src2][i];
		if(is_used[t-1])
			t=chromosome[src1][i];
		while (is_used[t-1]) t=chromosome[src2][j++];
		generation[target][i]=t;
		is_used[t-1]=1;
	}

}

void xover_uniform(int src1, int src2, int target)
{
	int t,i,j,is_used[MAX_LENGTH];//,temp_t;
	double t_fitness;

	for(j=0;j<g_length;j++) is_used[j]=0;
		
	for(i=0;i<g_length;i++)
	{
		j=0;
		if ((double(rand())/RAND_MAX)<=g_xover_uniform_p) 
		{
			t=chromosome[src1][i];
			if(is_used[t-1])
				t=chromosome[src2][i];

			/*
			temp_t=t;
			while(is_used[t-1])
			{
				j++;
				t=((temp_t+j-1)%g_length)+1;
				if(!is_used[t-1]) break;
				else
					t=((temp_t+g_length-j-1)%g_length)+1;
			}
			*/

			while (is_used[t-1]) t=chromosome[src1][j++];//t=(t+g_length-2)%g_length+1; ////t=chromosome[src1][j=(j+1)%g_length];//t=(t%g_length)+1; //t=chromosome[src1][j++];

			is_used[t-1]=1;
		}
		else 
		{
			t=chromosome[src2][i];
			if(is_used[t-1])
				t=chromosome[src1][i];
			
			/*
			temp_t=t;

			while(is_used[t-1])
			{
				j++;
				t=((temp_t+j-1)%g_length)+1;
				if(!is_used[t-1]) break;
				else
					t=((temp_t+g_length-j-1)%g_length)+1;
			}
			*/

			while (is_used[t-1]) t=chromosome[src2][j++];//t=(t+g_length-2)%g_length+1;// t=chromosome[src1][j++];//t=chromosome[src2][j=(j+1)%g_length];//t=(t%g_length)+1; //t=chromosome[src2][j++];
			
			is_used[t-1]=1;
		}

		generation[target][i]=t;
	}

	t_fitness=fitness_of_chromosome(src1);
	if((fitness_of_generation(target)>t) && (t<g_best_fitness))
		chromosome_tag[src1]=1;
	
	t_fitness=fitness_of_chromosome(src2);
	if((fitness_of_generation(target)>t_fitness) && (t<g_best_fitness))
		chromosome_tag[src2]=1;
}

void print_chromosome(int i)
{
	int j=0;
	for(j=0;j<g_length;j++)
		printf("(%d)",chromosome[i][j]);
	printf("\n");
}

void print_generation(int i)
{
	int j=0;
	for(j=0;j<g_length;j++)
		printf("(%d)",generation[i][j]);
	printf("\n");
}

void replacement()
{
	int i,tag_count=0,j=0,worst_index;
	double worst_fitness,t;

	for(i=0;i<g_population_size;i++)
		if(chromosome_tag[i]==1) tag_count++;

	while(tag_count-g_generation_size<0)
	{
		worst_index=0;
		worst_fitness=9999999;

		for(i=0;i<g_population_size;i++)
		{
			t=fitness_of_chromosome(i);
			if((chromosome_tag[i]!=1) && (t<worst_fitness) && (t<g_best_fitness)) 
			{
				worst_index=i;
				worst_fitness=fitness_of_chromosome(i);
			}
		}
		if(worst_fitness==9999999) break;
		chromosome_tag[worst_index]=1;
		tag_count++;
	}

	int startp,endp;

	startp=(int)(g_population_size*rand()/(RAND_MAX+1));
	endp = (startp+g_population_size-1) % g_population_size;

	i=startp;
	while(i++!=endp)
	{
		if(i>=g_population_size) i=0;
		if(chromosome_tag[i]==1)
		{
			replace_chromosome(i,j);
			chromosome_tag[i]=0;
			if(++j>=g_generation_size) return;
		}
	}

}

void replace_chromosome(int pop_index,int gen_index)
{
	int i,t_sum;
	double t;
	t=fitness_of_chromosome(pop_index);

	if(t>=fitness_of_generation(gen_index))  
	{
		if(t>=g_best_fitness) return;

		if ((double(rand())/RAND_MAX)>g_replace_bad_p) 
		{
			return;
		}

	}

	for(i=0;i<g_length;i++)
	{
		chromosome[pop_index][i]=generation[gen_index][i];
	}

	if(fitness_of_generation(gen_index)>g_best_fitness) 
	{
		g_best_fitness=fitness_of_generation(gen_index);
		save_best_chromosome(gen_index);

		printf("[%d].%d << BETTER CHROMOSOME >> : fitness=%f",g_test_num,g_gen,-g_best_fitness);
#ifdef PRINTPROCESS
		reportfile << "[" << g_test_num << "]." << g_gen << " << BETTER CHROMOSOME >> : fitness=" << -g_best_fitness;
#endif
		t_sum=0;
		for(i=0;i<6;i++)
			t_sum+=g_best_chromosome[chromosome_index[i_to_x[0]][i_to_y[0]][i]];

		if(g_best_fitness==0)
		{

			printf("\t(Solution)");
#ifdef PRINTPROCESS
			reportfile << "\t(Solution)";
#endif
			g_found=true;
			g_found_fitness=g_best_fitness;
			for(i=0;i<g_length;i++)
				g_sol_chromosome[i]=g_best_chromosome[i];
		}
		printf("\n");
#ifdef PRINTPROCESS
		reportfile << endl;
#endif
	}
}

void save_best_chromosome(int index)
{
	for(int i=0;i<g_length;i++)
	{
		g_best_chromosome[i]=generation[index][i];
	}
}

void mutation1(int index)
{
	int t1,t1_value,t2,t2_value;

	for(int i=0;i<g_length;i++)
		if ((double(rand())/RAND_MAX)<=g_mutation1_p) 
		{
			t1=i;
			t1_value=generation[index][i];

			t2_value=(int)(g_length*rand()/(RAND_MAX+1))+1;
			for(int j=0;j<g_length;j++)
			{
				if(generation[index][j]==t2_value)
				{
					t2=j;
					generation[index][t2]=t1_value;
					generation[index][t1]=t2_value;
				}
			}
		}
}

void mutation2(int index)
{
	int i,t,j,t_value[MAX_LENGTH],t_tag[MAX_LENGTH];

	for(j=0;j<g_length;j++)
		t_tag[MAX_LENGTH]=0;

	j=0;

	for(i=0;i<g_length;i++)
	{
		if ((double(rand())/RAND_MAX)<=g_mutation2_p) 
		{
			t_tag[j]=1;
			t_value[j]=generation[index][i];
			generation[index][i]=g_length+1;
			j++;
		}
	}

	for(i=0;i<g_length;i++)
	{
		if(generation[index][i]==g_length+1)
		{
			do
			{
				t=(int)(j*rand()/(RAND_MAX+1));
			} while(t_tag[t]==0);

			generation[index][i]=t_value[t];
			t_tag[t]=0;
		}
	}

}

bool spin(int index)
{
	bool ret=false,done;
	int t_hexa, t,j,i,k,temp[MAX_LENGTH];
	double t_fitness;
	t_fitness=fitness_of_generation(index);

	for(k=0;k<g_spin_count;k++)
	{
		t_hexa=(int)(g_hexa_count*rand()/(RAND_MAX+1));
		done=false;
		for(i=0;i<6;i++)
		{
			t=generation[index][chromosome_index[i_to_x[t_hexa]][i_to_y[t_hexa]][0]];
			for(j=0;j<5;j++)
			{
				generation[index][chromosome_index[i_to_x[t_hexa]][i_to_y[t_hexa]][j]]=generation[index][chromosome_index[i_to_x[t_hexa]][i_to_y[t_hexa]][j+1]];
			}
			generation[index][chromosome_index[i_to_x[t_hexa]][i_to_y[t_hexa]][5]]=t;

			if(fitness_of_generation(index)>t_fitness) 
			{
				done= true;
				t_fitness = fitness_of_generation(index);

				for(j=0;j<6;j++)
				{
					temp[j]=generation[index][chromosome_index[i_to_x[t_hexa]][i_to_y[t_hexa]][j]];
				}
			}
		}
		if(done)
		{
			if((double(rand())/RAND_MAX)<=g_do_spin_apply_p)
			{
				ret=true;
#ifdef CHECKING
				printf("Spin success: %f\n",t_fitness);
				reportfile << "Spin success: " << t_fitness << endl;
#endif
				for(j=0;j<6;j++)
				{
					generation[index][chromosome_index[i_to_x[t_hexa]][i_to_y[t_hexa]][j]]=temp[j];					
				}
			}
		}
	}
	return ret;
}

bool local_optimize(int index)
{
	int a,b,t,i,j,t_b,m; //,opt_chromosome[MAX_LENGTH];
	bool ret=false;
	double t_fitness;
	double local_best_generation[MAX_LENGTH];

	for(i=0;i<g_length;i++)
	{
		generation[MAX_LENGTH-1][i]=generation[index][i];
		local_best_generation[i]=generation[index][i];
	}

	t_fitness = fitness_of_generation(MAX_LENGTH-1);

	for(i=0;i<g_local_optimize_count;i++)
	{
		a=int(g_length*rand()/(RAND_MAX+1));

		//여기2opt 개선 
		
		b=int(g_opt_diff*rand()/(RAND_MAX+1))+1;

		if( (double(rand())/RAND_MAX)<0.5 )
		{
			b=-b;
		}

		t_b=((generation[MAX_LENGTH-1][a]+g_length+b) % g_length)+1;
		
		for(m=0;m<g_length;m++)
		{
			if(generation[MAX_LENGTH-1][m]==t_b)
			{
				b=m;
				break;
			}
		}

		/*
		do 
		{
			b=int(g_length*rand()/(RAND_MAX+1));
		}
		while(a==b);
*/

		t=generation[MAX_LENGTH-1][a];
		generation[MAX_LENGTH-1][a]=generation[MAX_LENGTH-1][b];
		generation[MAX_LENGTH-1][b]=t;

		if(t_fitness<fitness_of_generation(MAX_LENGTH-1))
		{
			ret=true;
			t_fitness = fitness_of_generation(MAX_LENGTH-1);
			for(j=0;j<g_length;j++)
			{
				local_best_generation[j]=generation[MAX_LENGTH-1][j];
			}

			if((double(rand())/RAND_MAX)>g_do_deep_better_optimize_p)
			{
				t=generation[MAX_LENGTH-1][a];
				generation[MAX_LENGTH-1][a]=generation[MAX_LENGTH-1][b];
				generation[MAX_LENGTH-1][b]=t;
			}

		}
		else
		{
			if((double(rand())/RAND_MAX)>g_do_deep_worse_optimize_p)
			{
				t=generation[MAX_LENGTH-1][a];
				generation[MAX_LENGTH-1][a]=generation[MAX_LENGTH-1][b];
				generation[MAX_LENGTH-1][b]=t;
			}
		}

	}
	if(ret=true)
	{
			for(j=0;j<g_length;j++) 
			{
				generation[index][j]=local_best_generation[j];
			}
	}
	return ret;
}

bool hexa_optimize(int index)
{
	int temp_hexa[6],tag[6],k,k2;
	bool ret=false;
	int t_hexa, t,i;
	double t_fitness;

	t_fitness=fitness_of_generation(index);

	for(k=0;k<g_hexa_optimize_count;k++)
	{

		t_hexa=(int)(g_hexa_count*rand()/(RAND_MAX+1));
		
		for(k2=0;k2<g_hexa_optimize_inner_count;k2++)
		{
			for(i=0;i<6;i++)
			{
				generation[MAX_LENGTH-1][chromosome_index[i_to_x[t_hexa]][i_to_y[t_hexa]][i]]=generation[index][chromosome_index[i_to_x[t_hexa]][i_to_y[t_hexa]][i]];
				temp_hexa[i]=generation[index][chromosome_index[i_to_x[t_hexa]][i_to_y[t_hexa]][i]];
				tag[i]=0;
			}
			
			for(int i=0;i<6;i++)
			{
				do
				{
					t=int(6*rand()/(RAND_MAX+1));
				} while(tag[t]==1);

				generation[MAX_LENGTH-1][chromosome_index[i_to_x[t]][i_to_y[t]][i]]=temp_hexa[t];
				tag[t]=1;
			}

			if(fitness_of_generation(MAX_LENGTH-1)>t_fitness)
			{
				t_fitness = fitness_of_generation(MAX_LENGTH-1);
#ifdef CHECKING
				printf("Hexa optimize success: %f\n",t_fitness);
				reportfile << "Hexa optimize success: " << t_fitness << endl;
#endif			
				for(i=0;i<6;i++)
				{
					generation[index][chromosome_index[i_to_x[t_hexa]][i_to_y[t_hexa]][i]]=generation[MAX_LENGTH-1][chromosome_index[i_to_x[t_hexa]][i_to_y[t_hexa]][i]];
				}
				ret=true;
			}
		}
	}
	return true;
}

double calculate_percentage()
{
	int cnt=0;

	for(int i=0;i<g_population_size;i++)
	{
		//printf("(%f, %f)",fitness_of_chromosome(i),g_best_fitness);

		if(fitness_of_chromosome(i)>=g_best_fitness) cnt++;
	}

	return double(cnt)/g_population_size;

}

void make_summary()
{
	double sum_fitness=0,sum_time=0,sum_sol=0;
	int cnt=0;

	////////////////////////////////////
	// Display
	////////////////////////////////////

	printf("\n\ntest#\tfitness\t\tsum\t\ttime\n");
	printf("---------------------------------------------------\n");

	reportfile << endl << endl << "test#\tfitness\t\tsum\t\ttime" << endl;
	reportfile << "---------------------------------------------------" << endl;

	for(int i=0;i<g_test_count;i++)
	{
		sum_fitness+=g_global_fitness[i];
		sum_time+=g_global_time[i];
		printf("%d\t%f",i+1,-g_global_fitness[i]);
		reportfile << i+1 << "\t" << -g_global_fitness[i];
		if(g_global_solution_sum[i]>0)
		{
			sum_sol+=g_global_solution_sum[i];
			printf("\t%d\t\t%f\n",g_global_solution_sum[i],g_global_time[i]);
			reportfile << "\t\t" << g_global_solution_sum[i] << "\t\t" << g_global_time[i] << endl;
			cnt++;
		}
		else
		{
			printf("\t\t\t%f\n",g_global_time[i]);
			reportfile << "\t\t\t\t" << g_global_time[i] << endl;
		}
			
	}

	printf("\n\n---------------------------------------------------\n");
	printf(    "Best fitness      : %f ( test# %d)\n",-g_global_best_fitness, g_global_best_fitness_test_num);
	if(cnt>0) printf(    "Best solution sum : %d ( test# %d)\n",g_global_best_solution_sum,g_global_best_solution_sum_test_num);
	printf(    "Shortest time     : %f ( test# %d)\n\n",g_global_best_time, g_global_best_time_test_num);

	printf(    "Average fitness   : %f\n",-sum_fitness/g_test_count);
	if(cnt>0) printf(    "Average sol. sum  : %f\n",sum_sol/cnt);
	printf(    "Average time      : %f\n",sum_time/g_test_count);	

	//report
	reportfile << endl << endl << "---------------------------------------------------" << endl;
	reportfile << "Best fitness      : " << -g_global_best_fitness << " ( test# " << g_global_best_fitness_test_num << ")" << endl;
	if(cnt>0) 
	{
		reportfile << "Best solution sum : " << g_global_best_solution_sum << " ( test# " << g_global_best_solution_sum_test_num << ")" << endl;
	}
	reportfile << "Shortest time     : " << g_global_best_time << " ( test# " << g_global_best_time_test_num << ")" << endl << endl;

	reportfile << "Average fitness   : " << -sum_fitness/g_test_count << endl;
	if(cnt>0) 
	{
		reportfile << "Average sol. sum  : " << sum_sol/cnt << endl;		
	}
	reportfile << "Average time      : " << sum_time/g_test_count << endl;	

	reportfile.close();

#ifdef USEDONEFILE
	donefile.open("c:\\done.ga");
	donefile << -sum_fitness/g_test_count << " " << -g_global_best_fitness << " " << sum_time/g_test_count << " " << g_global_best_time << endl;
	donefile.close();

	donefile2.open ("c:\\done.msg");
	donefile2 << "done" << endl;
	donefile2.close();
#endif

}

double get_population_average()
{
	double sum=0;

	for(int i=0;i<g_population_size;i++)
		sum+=fitness_of_chromosome(i);

	return sum/g_population_size;
}

// temp


	/* 2,3 BEST 
	int x,y,i,p=0,t_index=0;
	g_min_width=2;			// 지수귀문도에서 이어진 평행 육각형들의 시작 길이 ( 가장 짧다 )
	g_max_width=3;			// 지수귀문도에서 이어진 평행 육각형들의 가운데 길이 ( 가장 길다 )
	g_population_size=200;		// population 크기
	g_generation_size=80;		// generation 크기
	g_tournament_winner_p=0.6;
	g_xover_uniform_p=0.6;
	g_replace_bad_p=0.3;
	g_mutation_p=0.09;
	g_local_optimize_count=10;
	g_spin_count=1;
	g_deep_better_optimize=1;
	g_deep_worse_optimize=0;
	//g_optimize_loop_count=50;
	g_do_optimize_p=1;
	g_do_mutation_p=0;
*/

	/* 8,8 BEST
	int x,y,i,p=0,t_index=0;
	g_min_width=8;			// 지수귀문도에서 이어진 평행 육각형들의 시작 길이 ( 가장 짧다 )
	g_max_width=8;			// 지수귀문도에서 이어진 평행 육각형들의 가운데 길이 ( 가장 길다 )
	g_population_size=100;		// population 크기
	g_generation_size=80;		// generation 크기
	g_tournament_winner_p=0.6;
	g_xover_uniform_p=0.6;
	g_replace_bad_p=0.3;
	g_mutation_p=0.09;
	g_local_optimize_count=10;
	g_spin_count=1;
	g_deep_better_optimize=1;
	g_deep_worse_optimize=0;
	//g_optimize_loop_count=50;
	g_do_optimize_p=1;
	g_do_mutation_p=0;
*/
	/* 7,7 BEST 
	int x,y,i,p=0,t_index=0;
	g_min_width=7;			// 지수귀문도에서 이어진 평행 육각형들의 시작 길이 ( 가장 짧다 )
	g_max_width=7;			// 지수귀문도에서 이어진 평행 육각형들의 가운데 길이 ( 가장 길다 )
	g_population_size=100;		// population 크기
	g_generation_size=50;		// generation 크기
	g_tournament_winner_p=0.6;
	g_xover_uniform_p=0.6;
	g_replace_bad_p=0.3;
	g_mutation_p=0.09;
	g_local_optimize_count=10;
	g_spin_count=1;
	g_deep_better_optimize=1;
	g_deep_worse_optimize=0;
	//g_optimize_loop_count=50;
	g_do_optimize_p=1;
	g_do_mutation_p=0;
*/


	/* BEST 1,2 
	g_min_width=1;			// 지수귀문도에서 이어진 평행 육각형들의 시작 길이 ( 가장 짧다 )
	g_max_width=2;			// 지수귀문도에서 이어진 평행 육각형들의 가운데 길이 ( 가장 길다 )
	g_population_size=200;		// population 크기
	g_generation_size=30;		// generation 크기
	g_tournament_winner_p=0.6;
	g_xover_uniform_p=0.6;
	g_replace_bad_p=0.3;
	g_mutation_p=0.09;
	g_local_optimize_count=10;
	g_spin_count=1;
	g_deep_better_optimize=1;
	g_deep_worse_optimize=0;
	//g_optimize_loop_count=50;
	g_do_optimize_p=1;
	g_do_mutation_p=0;
*/
/* 1,1 ~ 6,6 까지BEST
	g_min_width=6;			// 지수귀문도에서 이어진 평행 육각형들의 시작 길이 ( 가장 짧다 )
	g_max_width=6;			// 지수귀문도에서 이어진 평행 육각형들의 가운데 길이 ( 가장 길다 )
	g_population_size=100;		// population 크기
	g_generation_size=30;		// generation 크기
	g_tournament_winner_p=0.6;
	g_xover_uniform_p=0.6;
	g_replace_bad_p=0.3;
	g_mutation_p=0.09;
	g_local_optimize_count=10;
	g_spin_count=1;
	g_deep_better_optimize=1;
	g_deep_worse_optimize=0;
	//g_optimize_loop_count=50;
	g_do_optimize_p=1;
	g_do_mutation_p=0;
*/
/////////////////////////////////////////////
	/* 3,5 BESTBESTBEST
	g_outputfile=".\\output.txt";
	g_graphfile=".\\graph.xls";

	g_min_width=3;			// 지수귀문도에서 이어진 평행 육각형들의 시작 길이 ( 가장 짧다 )
	g_max_width=5;			// 지수귀문도에서 이어진 평행 육각형들의 가운데 길이 ( 가장 길다 )
	
	g_population_size=50;		// population 크기
	g_generation_size=12;		// 또는 10 // generation 크기
	
	g_tournament_winner_p=1;	//0.9도 괜찮음
	g_xover_uniform_p=0.697;
	
	g_replace_bad_p=0.195;
	
	g_do_optimize_p=1;
	g_local_optimize_count=6000;
	g_do_deep_better_optimize_p=1;
	g_do_deep_worse_optimize_p=0;
	
	g_do_mutation1_p=0.1;
	g_mutation1_p=0.05;
	
	g_do_mutation2_p=0.12;
	g_mutation2_p=0.08;
	g_force_mutation2=0;

	g_spin_count=1000;
	g_do_spin_p=0;
	g_do_spin_apply_p=1;

	//종료조건
	g_converge_rate_p=0.99;
	g_converge_count=3;

	//테스트 시행회수
	g_test_count=2;
*/
/*	비교적 괜찮은 3,5
	int x,y,i,p=0,t_index=0;
	g_min_width=3;			// 지수귀문도에서 이어진 평행 육각형들의 시작 길이 ( 가장 짧다 )
	g_max_width=5;			// 지수귀문도에서 이어진 평행 육각형들의 가운데 길이 ( 가장 길다 )
	
	g_population_size=50;		// population 크기
	g_generation_size=12;		// 또는 10 // generation 크기
	
	g_tournament_winner_p=1;	//0.9도 괜찮음
	g_xover_uniform_p=0.69;
	
	g_replace_bad_p=0.2;
	
	g_do_optimize_p=1;
	g_local_optimize_count=6000;
	g_do_deep_better_optimize_p=1;
	g_do_deep_worse_optimize_p=0.1;
	
	g_do_mutation1_p=0.1;
	g_mutation1_p=0.05;
	
	g_do_mutation2_p=0.12;
	g_mutation2_p=0.08;
	g_force_mutation2=0;

	g_spin_count=1000;
	g_do_spin_p=0.1;
*/


	/* 3,5 189로 찾았음 BEST
	int x,y,i,p=0,t_index=0;
	g_min_width=3;			// 지수귀문도에서 이어진 평행 육각형들의 시작 길이 ( 가장 짧다 )
	g_max_width=5;			// 지수귀문도에서 이어진 평행 육각형들의 가운데 길이 ( 가장 길다 )
	
	g_population_size=50;		// population 크기
	g_generation_size=12;		// 또는 10 // generation 크기
	
	g_tournament_winner_p=0.9;	//0.9도 괜찮음
	g_xover_uniform_p=0.69;
	
	g_replace_bad_p=0.2;
	
	g_do_optimize_p=1;
	g_local_optimize_count=6000;
	g_do_deep_better_optimize_p=1;
	g_do_deep_worse_optimize_p=0;
	
	g_do_mutation1_p=0.1;
	g_mutation1_p=0.05;
	
	g_do_mutation2_p=0.1;
	g_mutation2_p=0.08;
	g_force_mutation2=0;

	g_spin_count=1000;
	g_do_spin_p=0.1;
*/


	/* 3,5 찾아냄188 /
	int x,y,i,p=0,t_index=0;
	g_min_width=3;			// 지수귀문도에서 이어진 평행 육각형들의 시작 길이 ( 가장 짧다 )
	g_max_width=5;			// 지수귀문도에서 이어진 평행 육각형들의 가운데 길이 ( 가장 길다 )
	
	g_population_size=50;		// population 크기
	g_generation_size=20;		// 또는 10 // generation 크기
	
	g_tournament_winner_p=0.9;	//0.9도 괜찮음
	g_xover_uniform_p=0.6;
	
	g_replace_bad_p=0.2;
	
	g_do_optimize_p=1;
	g_local_optimize_count=6000;
	g_do_deep_better_optimize_p=1;
	g_do_deep_worse_optimize_p=0;
	
	g_do_mutation1_p=0.1;
	g_mutation1_p=0.05;
	
	g_do_mutation2_p=0.1;
	g_mutation2_p=0.08;
	g_force_mutation2=0;

	g_spin_count=1000;
	g_do_spin_p=0.1;
*/

	/* 3,4 BEST /
	int x,y,i,p=0,t_index=0;
	g_min_width=3;			// 지수귀문도에서 이어진 평행 육각형들의 시작 길이 ( 가장 짧다 )
	g_max_width=4;			// 지수귀문도에서 이어진 평행 육각형들의 가운데 길이 ( 가장 길다 )
	
	g_population_size=150;		// population 크기
	g_generation_size=120;		// generation 크기
	
	g_tournament_winner_p=0.9;	//0.9도 괜찮음
	g_xover_uniform_p=0.6;
	
	g_replace_bad_p=0.15;
	
	g_do_optimize_p=1;
	g_local_optimize_count=6000;
	g_do_deep_better_optimize_p=1;
	g_do_deep_worse_optimize_p=0;
	
	g_do_mutation1_p=0.1;
	g_mutation1_p=0.05;
	
	g_do_mutation2_p=0.1;
	g_mutation2_p=0.05;
	g_force_mutation2=0;

	g_spin_count=1000;
	g_do_spin_p=0.1;
*/

/*
	 2,3 까지 best 4,5까지 괜찮음
	int x,y,i,p=0,t_index=0;
	g_min_width=2;			// 지수귀문도에서 이어진 평행 육각형들의 시작 길이 ( 가장 짧다 )
	g_max_width=3;			// 지수귀문도에서 이어진 평행 육각형들의 가운데 길이 ( 가장 길다 )
	g_population_size=100;		// population 크기
	g_generation_size=30;		// generation 크기
	g_tournament_winner_p=0.6;	//0.9도 괜찮음
	g_xover_uniform_p=0.6;
	g_replace_bad_p=0.1;
	g_mutation1_p=0.05;
	g_mutation2_p=0.1;
	g_local_optimize_count=300;
	g_do_deep_better_optimize=1;
	g_do_deep_worse_optimize=0;
	g_do_optimize_p=1;
	g_do_mutation1_p=0.1;
	g_do_mutation2_p=0.1;
	g_force_mutation2=0;
*/
