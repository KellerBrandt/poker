#include "KuhnPoker.h"
#include "CFR.h"

int main() {
    KuhnPoker kp;
    CFR cfr;
    cfr.runVanillaCFR(kp, 100000);
    kp.print();
}