#include <stdio.h>
#define N 5  // จำนวนโปรเซส

typedef struct{
    int BurtT;
    int ArrivalT;
    int Priority;
}Process;

typedef struct{
    int indexP;
    int BurtT;
    int Priority;
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
int NT = 0;      // number time
int NP = N;      // number process

void push(int index, int BTimeLeft,int priority){
    SP++;
    Q[SP].indexP    = index;               // เก็บโปรเซสไว้ในคิว
    Q[SP].BurtT     = BTimeLeft;           // เก็บเวลาการทำงานของโปรเซสที่เหลือไว้ในคิว
    Q[SP].Priority  = priority;            // เก็บลำดับความสำคัญของโปรเซส
}
void pop(){
    if(SP == 0) printf("POP Over !!!!\n");
    else        SP--;
}

int minPriority(int index){                 // หาโปรเซสที่มีลำดับความสำคัญสูงสุด ในกรณี ที่โปรเซสเกิดพร้อมกัน
    int temp[20], n = 0;
    int index2 = index;
    int min = P[index].Priority;
    for (int i = 1; i <= NP; i++) {
        if(P[index].ArrivalT == P[i].ArrivalT){
            temp[n++] = i;
            if(P[i].Priority < min){
                min = P[i].Priority;
                index2 = i;
            }
        }
    }
    for (int j = 0; j < n; j++) {      // นำโปรเตัวที่เหลือไปต่อคิว ยกเว้นโปรเซสที่มีลำดับความสำคัญสูงสุด
        if(temp[j] != index2){
            push(temp[j], P[temp[j]].BurtT, P[temp[j]].Priority);
        }
    }
    return index2;                     // return โปรเซสที่มีลำดับความสำคัญสูงสุด
}

int duplicate(int index){               // ตรวจดูว่ามีโปรเซสเเกิดในเวลาเดียวกันหรือไม่
    int count = 0;
    for (int i = 1; i <= NP; i++)
        if(P[index].ArrivalT == P[i].ArrivalT)
            count++;
    if (count > 1)                      // ถ้ามีโปรเซสเกิดในเวลาเดียวกัน
        return minPriority(index);      // หาโปรเซสที่มีลำดับความสำคัญสูงสุด
    else                                // ถ้าไม่มีโปรเซสเกิดในเวลาเดียวกัน
        return index;
}

Queue sort_Q(){
    Queue temp;
    for (int i = 1; i <=SP ; i++) {
        for (int j = i+1; j <=SP ; j++) {
            if(Q[i].Priority < Q[j].Priority){
                temp = Q[i];
                Q[i] = Q[j];
                Q[j] = temp;
            }
        }
    }
    return Q[SP];
}

void Pre_Priority(){
    int runningP, index, END_P = 0;
    for (int i = 0; i <NT ; i++) {                  // i แทน time(เวลา)
        for (int j = 1; j <=NP ; j++){
            if(i == P[j].ArrivalT) {                // ณ เวลาที่ i มี process[j] เกิดขึ้น
                index = duplicate(j);               // ตรวจสอบว่า ณ เวลานี่(i) มีโปรเซสเกิดขึ้นพร้อมกัน ใช่หรือไม่
                if(i >= END_P && P[index].Priority < sort_Q().Priority){ // ทรัพยากรว่าง(ไม่มีโปรเซสไหนที่กำลังทำงาน) = โปรเซสสามารเข้าไปทำงานได้
                    // แต่ ในกรณีที่ ทรัพยากรว่างพอดี แล้วมีโปรเซสเเกิดใหม่แต่มีลำดับควาสำคัญน้อยกว่าโปรเซสในคิว ให้โปรเซสที่เกิดใหม่ไปเก็บในคิว
                    END_P = i + P[index].BurtT;     // เวลาจบการทำงานของโปรเซส
                    runningP = index;               // ตอนนี้มีโปรเซสหนึ่ง กำลังอยู่ในสถานะทำงาน
                    Gantt[NG].indexP = index;
                    Gantt[NG].startP = i;           //เก็บค่าเวลาที่โปรเซสได้เริ่มทำงาน
                    NG++;
                    break;
                }else{                              // ทรัพยากรวไม่ว่าง(มีโปรเซสกำลังทำงาน)
                    if(P[index].Priority < P[runningP].Priority ){  // ถ้าโปรเซสเกิดใหม่ มีลำดับความสำคัญสูงกว่า โปรเซสที่กำลังทำงาน
                        push(runningP,(P[runningP].BurtT - i),P[runningP].Priority);    // เก็บโปรเซสที่กำลังทำงานไว้ในคิวก่อน
                        END_P = i + P[index].BurtT;
                        runningP = index;                           // โปรเซสใหม่อยู่ในสถานะทำงาน
                        Gantt[NG].indexP = index;
                        Gantt[NG].startP = i;
                        NG++;
                        break;
                    } else{                                         // ถ้าโปรเซสเกิดใหม่ มีลำดับความสำคัญน้อยกว่าหรือเท่ากับ โปรซสที่กำลังทำงาน
                        push(index, P[index].BurtT, P[index].Priority);
                    }
                }
            }
        }
        if(i >= END_P && SP != 0){                                 // ถ้าไม่มีโปรเซสไหนเกิดขึ้นใหม่ แต่ยังมีโปรเซสเหลือในอยู่ในคิว
            Queue index;
            index = sort_Q();
            pop();
            END_P = i+index.BurtT;
            runningP = index.indexP;
            Gantt[NG].indexP = index.indexP;
            Gantt[NG].startP = i;
            NG++;
        }
    }
}

float waitProcess(int indexP){                                  // คำนวณหา เวลาที่โปรเซสรอ
    int count = 0;
    float waitT =0, end = 0;
    for (int i = 0; i <NG ; i++) {
        if(Gantt[i].indexP == indexP){
            if(count == 0){                                     // เวลาโปรเซสได้เข้าทำงาน - เวลาเเกิดของโปรเซส
                waitT += (float)Gantt[i].startP - P[indexP].ArrivalT;
                end    = (float)Gantt[i+1].startP;
                count++;
            }else{
                waitT += (float)Gantt[i].startP - end;          // เวล่ที่โปรเซสได้เข้าทำงานอีกครั้ง - เวลาที่โปรเซสจบการทำงานครั้งก่อน
                end    = (float)Gantt[i+1].startP;
            }
        }
    }
    return waitT;
}

void calNT(){
    int sumBurt = 0;
    int minArrival = P[1].ArrivalT;
    for (int i = 1; i <=NP; ++i) {
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
    printf("# Priority (SJF Preemptive)\n");
    printf("Sequence process is :");
    Pre_Priority();
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
