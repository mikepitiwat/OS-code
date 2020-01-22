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
int NG = 0;      // number Gantt_chart
int NT = 24;     // number time
int NP = N;      // number process

void push(int index){
    SP++;
    Q[SP] = index;
}
void pop(){
    if(SP == 0) printf("POP Over !!!!\n");
    else        SP--;
}

int duplicate(int index){               //  เช็คว่ามีการเกิดของโปรเซสเวลาเดียวกันไหม
    int count = 0;
    for (int i = 1; i <= NP; i++)
        if(P[index].ArrivalT == P[i].ArrivalT)
            count++;
    if (count > 1)  return 1;           // ถ้ามีการเกิดพร้อมกัน   return 1
    else            return 0;           // ถ้าไม่มีการเกิดพร้อมมกัน return 0
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
            push(temp[j]);
        }
    }
    return index2;                     // return โปรเซสที่ใช้เวลาในการทำงานน้อยที่สุด
}

int status(int index){
    if(duplicate(index) == 0){          // ถ้าไม่ซ้ำ แปลว่าไม่มี process เกิดในเวลาเดียวกัน
        if(SP == 0){                    // ไม่มี process ในคิว
            return index;
        }else{                          // มี process ในคิว
            push(index);
            return 0;
        }
    }else{                              // ถ้าซ้ำ แปลว่ามี process เกิดเวลาเดียวกัน
        if(SP == 0){                    // ไม่มี process ในคิว
            return minBurt(index);
        }else{                          // มี process ในคิว
            for (int i = 1; i <= NP; i++)
                if(P[index].ArrivalT == P[i].ArrivalT)
                    push(i);
            return 0;
        }
    }
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

void Non_PreSJF(){
    int index, END_P = 0;
    for (int i = 0; i <NT ; ++i) {                  // i แทน time(เวลา)
        for (int j = 1; j <=NP ; ++j) {
            if(i == P[j].ArrivalT) {                // ณ เวลาที่ i มี process[j] เกิดขึ้น
                index = status(j);                  // ตรวจสถานะ ต่างๆ เช่น การเกิดพร้อมกัน หรือ การต่อคิว  return0 คือ โปรเซสไปต่อคิว ไม่มีโปรเซสไหนออกมา
                if(i >= END_P){                     // แปลว่าถ้าไม่มีโปรเซสไหนอยู่ในการทำงาน
                    if(index == 0 && SP != 0){      // ถ้าไม่มีโปรเซสไหนออกมา แต่ในคิวยังมีโปรเซสรออยู่
                        index = sort_Q();
                        pop();
                    }
                    END_P = i + P[index].BurtT;     // เวลาจบการทำงานของโปรเซส
                    Gantt[NG]       = index;
                    P[index].start  = i;
                    P[index].end    = END_P;
                    NG++;
                    break;
                } else{                             //ถ้ามีโปรเซสทำงานอยู่
                    if(index != 0){                 //แต่มีโปรเซสเกิดใหม่ต้องการทำงาน
                        push(index);
                    }
                }
            }
        }
        if(i >= END_P && SP != 0){                  // ถ้าไม่มีโปรเซสไหนทำงาน แต่ใน Q ยังมีโปรเซสรออยู่
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

int main(){
    printf("# Mr Pitiwat Promnimit ID:61543206020-5\n");
    printf("# OUTPUT LAB6 CPU Scheduling\n");
    printf("# Non Preemptive SJF\n");
    printf("Sequence process is :");
    Non_PreSJF();
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
