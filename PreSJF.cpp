#include <stdio.h>
#define N 5  // จำนวนโปรเซส

typedef struct{
    int BurtT;
    int ArrivalT;
    int Priority;
    int start;
    int end;
}Process;

typedef struct{
    int indexP;
    int BurtT;
}Queue;
typedef struct{
    int indexP;
    int startP;
}Gantt_C;

//Process  burt time , Arrival time , Priority
Process P[N+1]  =  {{0},
        /*P1*/ {    9     ,       1      ,      3},     // P1 = P[1]
        /*P2*/ {    3     ,       1      ,      5},     // P2 = P[2]
        /*P3*/ {    5     ,       3      ,      1},     // P3 = P[3]
        /*P4*/ {    4     ,       4      ,      4},     // P4 = P[4]
        /*P5*/ {    2     ,       7      ,      2}};    // P5 = P[5]


Gantt_C Gantt[20];
Queue   Q[20];   // คิว
int SP = 0;      // ชี้ค่าในคิว
int NG = 0;      // number Gantt_chart
int NT = 30;     // number time
int NP = N;      // number process

void push(int index, int BTimeLeft){
    SP++;
    Q[SP].indexP = index;
    Q[SP].BurtT  = BTimeLeft;
}
void pop(){
    if(SP == 0) printf("POP Over !!!!\n");
    else        SP--;
}

int minBurt(int index){                 // หาค่าเวลาโปรเซสที่ใช้เวลาทำงานน้อยที่สุด ในกรณี ที่โปรเซสเกิดพร้อมกัน
    int temp[20], n = 0;
    int index2 = index;
    int min = P[index].BurtT;
    for (int i = 1; i <= NP; i++) {
        if(P[index].ArrivalT == P[i].ArrivalT){
            temp[n++] = i;
            if(P[i].BurtT < min){
                min = P[i].BurtT;
                index2 = i;
            }
        }
    }
    for (int j = 0; j < n; j++) {      // นำ process ตัวที่เหลือไปต่อคิว
        if(temp[j] != index2){
            push(temp[j],P[temp[j]].BurtT);
        }
    }
    return index2;                     // return โปรเซสที่ใช้เวลาในการทำงานน้อยที่สุด
}

int duplicate(int index){
    int count = 0;
    for (int i = 1; i <= NP; i++)
        if(P[index].ArrivalT == P[i].ArrivalT)
            count++;
    if (count > 1)
        return minBurt(index);
    else
        return index;
}

Queue sort_Q(){
    Queue temp;
    for (int i = 1; i <=SP ; i++) {
        for (int j = i+1; j <=SP ; j++) {
            if(Q[i].BurtT < Q[j].BurtT){  // Q[i].BurtT < Q[i].BurtT
                temp = Q[i];
                Q[i] = Q[j];
                Q[j] = temp;
            }
        }
    }
    return Q[SP];
}

void PreSJF(){
    int runningP, index, END_P = 0;
    for (int i = 0; i <NT ; i++) {
        for (int j = 1; j <=NP ; j++){
            if(i == P[j].ArrivalT) {
                index = duplicate(j);
                if(i >= END_P){
                    END_P = i + P[index].BurtT;// เวลาจบการทำงานของโปรเซส
                    runningP = index;
                    Gantt[NG].indexP = index;
                    Gantt[NG].startP = i;
                    NG++;
                    break;
                }else{
                    if(P[index].BurtT < (P[runningP].BurtT - i) ){
                        push(runningP, (P[runningP].BurtT - i));
                        END_P = i + P[index].BurtT;
                        runningP = index;
                        Gantt[NG].indexP = index;
                        Gantt[NG].startP = i;
                        NG++;
                        break;
                    } else{
                        push(index, P[index].BurtT);
                    }
                }
            }
        }
        if(i >= END_P && SP != 0){
            Queue index;
            index = sort_Q();
            pop();
            END_P = i+index.BurtT;  // q[index].b
            runningP = index.indexP;
            Gantt[NG].indexP = index.indexP;
            Gantt[NG].startP = i;
            NG++;
        }
    }
}

float waitProcess(int indexP){
    int count = 0;
    float waitT =0, end = 0;
    for (int i = 0; i <NG ; i++) {
        if(Gantt[i].indexP == indexP){
            if(count == 0){
                waitT += (float)Gantt[i].startP - P[indexP].ArrivalT;
                end    = (float)Gantt[i+1].startP;
                count++;
            }else{
                waitT += (float)Gantt[i].startP - end;
                end    = (float)Gantt[i+1].startP;
            }
        }
    }
    return waitT;
}

int main(){
    printf("# Mr Pitiwat Promnimit ID:61543206020-5\n");
    printf("# OUTPUT LAB6 CPU Scheduling\n");
    printf("# Preemptive SJF\n");
    printf("Sequence process is :");
    PreSJF();
    for (int i = 0; i <NG ; i++) {
        printf("P%d", Gantt[i].indexP);
        if(i<NG-1)
            printf("->");
    }
    printf("\n------------------------------------------------------------\n");
    printf("Wait time of process (millisecond)\n");
    for (int i = 1; i <= NP; i++) {
        printf("| P%-10d", i);
    }
    printf("\n");
    float sum=0;
    float avgTime;
    for (int i = 1; i <= NP; i++) {
        printf("| %-11.2f",waitProcess(i));
        sum += waitProcess(i);
    }
    avgTime = sum/NP;
    printf("\nAverage time is %.2f", avgTime);
    printf("\nTurnaround time\n");
    for (int i = 1; i <= NP; i++) {
        printf("|P%d = %-6.2fms ", i, waitProcess(i) + P[i].BurtT);
    }
    printf("\n------------------------------------------------------------\n");
    return 0;
}
