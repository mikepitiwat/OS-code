#include <stdio.h>
#define N 5  // จำนวนโปรเซส

typedef struct{
    int BurtT;
    int ArrivalT;
    int Priority;
    int start;
    int end;
}Process;

     //Process  burt time , Arrival time , Priority
Process P[N+1]  =  {{0},
        /*P1*/ {    9     ,       1      ,      3},     // P1 = P[1]
        /*P2*/ {    3     ,       1      ,      5},     // P2 = P[2]
        /*P3*/ {    5     ,       3      ,      1},     // P3 = P[3]
        /*P4*/ {    4     ,       4      ,      4},     // P4 = P[4]
        /*P5*/ {    2     ,       7      ,      2}};    // P5 = P[5]

int Gantt[20];
int Q[99];       // คิว
int SP = 0;      // ชี้ค่าในคิว
int NG = 0;      // number Gantt chart
int NT = 0;     // number time
int NP = N;      // number process

void push(int index){
    SP++;
    Q[SP] = index;                      // เก็บโปรเซสไว้ในคิว
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
    for (int j = 0; j < n; j++) {      // นำโปรเตัวที่เหลือไปต่อคิว ยกเว้นโปรเซสที่ใช้เวลาทำงานน้อยที่สุด
        if(temp[j] != index2){
            push(temp[j]);
        }
    }
    return index2;                     // return โปรเซสที่ใช้เวลาในการทำงานน้อยที่สุด
}
int duplicate(int index){              // ตรวจดูว่ามีโปรเซสเเกิดในเวลาเดียวกันหรือไม่
    int count = 0;
    for (int i = 1; i <= NP; i++)
        if(P[index].ArrivalT == P[i].ArrivalT)
            count++;
    if (count > 1)                     // ถ้ามีโปรเซสเกิดในเวลาเดียวกัน
        return minBurt(index);         // หาโปรเซสที่ใช้เวลทำงานน้อยที่สุด
    else                               // ถ้าไม่มีโปรเซสเกิดในเวลาเดียวกัน
        return index;
}

int sort_Q(){
    int temp;
    for (int i = 1; i <=SP ; i++) {
        for (int j = i+1; j <=SP ; j++) {
            if(P[Q[i]].BurtT < P[Q[j]].BurtT){
                temp = Q[i];
                Q[i] = Q[j];
                Q[j] = temp;
            }
        }
    }
    return Q[SP];
}

void SJF_NonPre(){
    int index, END_P = 0;
    for (int i = 0; i <NT ; ++i) {                  // i แทน time(เวลา)
        for (int j = 1; j <=NP ; ++j) {
            if(i == P[j].ArrivalT) {                // ณ เวลาที่ i มี process[j] เกิดขึ้น
                index = duplicate(j);               // ตรวจสอบว่า ณ เวลานี่(i) มีโปรเซสเกิดขึ้นพร้อมกัน ใช่หรือไม่
                if(i >= END_P){                     // ทรัพยากรว่าง(ไม่มีโปรเซสไหนที่กำลังทำงาน) = โปรเซสสามารเข้าไปทำงานได้
                    END_P = i + P[index].BurtT;     // เวลาจบการทำงานของโปรเซส
                    Gantt[NG]       = index;
                    P[index].start  = i;            //เก็บค่าเวลาที่โปรเซสได้เริ่มทำงาน
                    P[index].end    = END_P;        //เก็บค่าเวลาที่โปรเซสได้จบทำงาน
                    NG++;
                    break;
                } else{                             // ทรัพยากรวไม่ว่าง(มีโปรเซสกำลังทำงาน)
                    push(index);                    // เก็บโปรเซสไว้ในคิว
                }
            }
        }
        if(i >= END_P && SP != 0){                  // ถ้าไม่มีโปรเซสไหนเกิดขึ้นใหม่ แต่ยังมีโปรเซสเหลือในอยู่ในคิว
            index = sort_Q();
            pop();
            END_P = i + P[index].BurtT;
            Gantt[NG]       = index;
            P[index].start  = i;
            P[index].end    = END_P;
            NG++;
        }
    }
}

void calNT(){
    int sumBurt = 0;
    int minArrival = P[1].ArrivalT;
    for (int i = 1; i < NP; ++i) {
        if(P[i].ArrivalT < minArrival){
            minArrival = P[i].ArrivalT;
        }
        sumBurt += P[i].BurtT;
    }
    NT = minArrival + sumBurt;
}

int main(){
    calNT();
    printf("# Mr Pitiwat Promnimit ID:61543206020-5\n");
    printf("# OUTPUT LAB6 CPU Scheduling\n");
    printf("# SJF Non Preemptive \n");
    printf("Sequence process is :");
    SJF_NonPre();
    for (int i = 0; i <NG ; i++) {
        printf("P%d", Gantt[i]);
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
        printf("| %-11.2f",(float)P[i].start - P[i].ArrivalT);
        sum += (float)P[i].start - P[i].ArrivalT;
    }
    avgTime = sum/NP;
    printf("\nAverage time is %.2f", avgTime);
    printf("\nTurnaround time\n");
    for (int i = 1; i <= NP; i++) {
        printf("|P%d = %-6.2fms ", i, ((float) P[i].start - P[i].ArrivalT) + P[i].BurtT);
    }
    printf("\n------------------------------------------------------------\n");
    return 0;
}
