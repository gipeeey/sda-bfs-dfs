#include "student_profile.h"

StudentProfile getSampleProfileDimas() {
    StudentProfile p;
    p.name = "Dimas";
    p.masteredModules = {"M1"};
    p.targetModule = "M7";
    p.timeBudgetHours = 45.0;
    return p;
}

StudentProfile getSampleProfileSari() {
    StudentProfile p;
    p.name = "Sari";
    p.masteredModules = {"M1", "M2", "M3"};
    p.targetModule = "M7";
    p.timeBudgetHours = 20.0;
    return p;
}
