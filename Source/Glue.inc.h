namespace acclimation {
   const char* name() {return PSTR("Acclimation");}
   int cardinality() {return APP_CARDINALITY_UNDEFINED;}
   void registerVars();
   void whenCreated();
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr);
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2) {return 0;}
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered();
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b);
   int evalCondition(int a, uint16_t* array) {return 0;}
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 0;}
}
namespace admin {
   const char* name() {return PSTR("Admin");}
   int cardinality();
   void registerVars();
   void whenCreated();
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr) {}
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2) {return 0;}
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered() {}
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b);
   int evalCondition(int a, uint16_t* array) {return 0;}
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 0;}
}
namespace aqua {
   const char* name() {return PSTR("Aqua");}
   int cardinality() {return APP_CARDINALITY_UNDEFINED;}
   void registerVars() {}
   void whenCreated() {}
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr) {}
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2) {return 0;}
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered() {}
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b) {return 0;}
   int evalCondition(int a, uint16_t* array) {return 0;}
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 0;}
}
namespace beeper {
   const char* name() {return PSTR("Beeper");}
   int cardinality() {return APP_CARDINALITY_UNDEFINED;}
   void registerVars() {}
   void whenCreated() {}
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr) {}
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2) {return 0;}
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered() {}
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b) {return 0;}
   int evalCondition(int a, uint16_t* array) {return 0;}
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 0;}
}
namespace berlinuhr {
   const char* name() {return PSTR("Mengenlehreuhr");}
   int cardinality() {return APP_CARDINALITY_UNDEFINED;}
   void registerVars() {}
   void whenCreated() {}
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr);
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2) {return 0;}
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered() {}
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b) {return 0;}
   int evalCondition(int a, uint16_t* array) {return 0;}
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 0;}
}
namespace button {
   const char* name() {return PSTR("Button");}
   int cardinality() {return APP_CARDINALITY_UNDEFINED;}
   void registerVars();
   void whenCreated();
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr);
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2);
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered();
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b);
   int evalCondition(int a, uint16_t* array);
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 0;}
}
namespace caluchitos {
   const char* name() {return PSTR("Caluchitos");}
   int cardinality() {return APP_CARDINALITY_UNDEFINED;}
   void registerVars() {}
   void whenCreated() {}
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr) {}
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2) {return 0;}
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered() {}
   void startScreen() {}
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b) {return 0;}
   int evalCondition(int a, uint16_t* array) {return 0;}
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 0;}
}
namespace clock {
   const char* name() {return PSTR("Clock");}
   int cardinality();
   void registerVars();
   void whenCreated() {}
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr);
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2);
   void setConditionParameter(int kind, int param);
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered();
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b) {return 0;}
   int evalCondition(int a, uint16_t* array);
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 0;}
}
namespace debug {
   const char* name() {return PSTR("Debug");}
   int cardinality() {return APP_CARDINALITY_UNDEFINED;}
   void registerVars() {}
   void whenCreated() {}
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr) {}
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2) {return 0;}
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered() {}
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b) {return 0;}
   int evalCondition(int a, uint16_t* array) {return 0;}
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 0;}
}
namespace desktop {
   const char* name() {return PSTR("Desktop");}
   int cardinality();
   void registerVars() {}
   void whenCreated() {}
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr) {}
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2) {return 0;}
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered() {}
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b) {return 0;}
   int evalCondition(int a, uint16_t* array) {return 0;}
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 0;}
}
namespace dice {
   const char* name() {return PSTR("Dice");}
   int cardinality() {return APP_CARDINALITY_UNDEFINED;}
   void registerVars() {}
   void whenCreated() {}
   void monitor(int x,int y,int w,int h);
   void timeSlice(int fg, frequency fr) {}
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2);
   void setConditionParameter(int kind, int param);
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c);
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered() {}
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b) {return 0;}
   int evalCondition(int a, uint16_t* array);
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 1;}
}
namespace helloWorld {
   const char* name() {return PSTR("Hello World");}
   int cardinality() {return APP_CARDINALITY_UNDEFINED;}
   void registerVars() {}
   void whenCreated() {}
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr) {}
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2) {return 0;}
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered() {}
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b) {return 0;}
   int evalCondition(int a, uint16_t* array) {return 0;}
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 0;}
}
namespace led {
   const char* name() {return PSTR("LED");}
   int cardinality() {return APP_CARDINALITY_UNDEFINED;}
   void registerVars();
   void whenCreated();
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr);
   const char* actionsDef(int kind);
   const char* conditionsDef(int kind2) {return 0;}
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param);
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered();
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b);
   int evalCondition(int a, uint16_t* array) {return 0;}
   void performAction(int a, uint16_t* array);
   int hasMonitor() {return 0;}
}
namespace monitor {
   const char* name() {return PSTR("Monitor");}
   int cardinality();
   void registerVars() {}
   void whenCreated() {}
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr);
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2) {return 0;}
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered() {}
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b) {return 0;}
   int evalCondition(int a, uint16_t* array) {return 0;}
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 0;}
}
namespace ph {
   const char* name() {return PSTR("PH");}
   int cardinality() {return APP_CARDINALITY_UNDEFINED;}
   void registerVars();
   void whenCreated();
   void monitor(int x,int y,int w,int h);
   void timeSlice(int fg, frequency fr);
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2);
   void setConditionParameter(int kind, int param);
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c);
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered();
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b) {return 0;}
   int evalCondition(int a, uint16_t* array);
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 1;}
}
namespace portsManager {
   const char* name() {return PSTR("Pin Manager");}
   int cardinality();
   void registerVars() {}
   void whenCreated() {}
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr);
   const char* actionsDef(int kind);
   const char* conditionsDef(int kind2);
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered();
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b);
   int evalCondition(int a, uint16_t* array);
   void performAction(int a, uint16_t* array);
   int hasMonitor() {return 0;}
}
namespace puoooc {
   const char* name() {return PSTR("Puoooc");}
   int cardinality() {return APP_CARDINALITY_UNDEFINED;}
   void registerVars();
   void whenCreated();
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr);
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2) {return 0;}
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered();
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b) {return 0;}
   int evalCondition(int a, uint16_t* array) {return 0;}
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 0;}
}
namespace reservoir {
   const char* name() {return PSTR("Reservoir");}
   int cardinality() {return APP_CARDINALITY_UNDEFINED;}
   void registerVars();
   void whenCreated();
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr);
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2) {return 0;}
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered();
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b);
   int evalCondition(int a, uint16_t* array) {return 0;}
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 0;}
}
namespace solenoid {
   const char* name() {return PSTR("Solenoid");}
   int cardinality() {return APP_CARDINALITY_UNDEFINED;}
   void registerVars();
   void whenCreated();
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr);
   const char* actionsDef(int kind);
   const char* conditionsDef(int kind2) {return 0;}
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered();
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b) {return 0;}
   int evalCondition(int a, uint16_t* array) {return 0;}
   void performAction(int a, uint16_t* array);
   int hasMonitor() {return 0;}
}
namespace tds {
   const char* name() {return PSTR("TDS");}
   int cardinality() {return APP_CARDINALITY_UNDEFINED;}
   void registerVars();
   void whenCreated();
   void monitor(int x,int y,int w,int h);
   void timeSlice(int fg, frequency fr);
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2);
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered() {}
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b) {return 0;}
   int evalCondition(int a, uint16_t* array);
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 1;}
}
namespace th {
   const char* name() {return PSTR("TH");}
   int cardinality() {return APP_CARDINALITY_UNDEFINED;}
   void registerVars();
   void whenCreated();
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr);
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2) {return 0;}
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered();
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b);
   int evalCondition(int a, uint16_t* array) {return 0;}
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 0;}
}
namespace timer {
   const char* name() {return PSTR("Timer");}
   int cardinality() {return APP_CARDINALITY_UNDEFINED;}
   void registerVars();
   void whenCreated();
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr);
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2);
   void setConditionParameter(int kind, int param);
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered() {}
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b) {return 0;}
   int evalCondition(int a, uint16_t* array);
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 0;}
}
namespace waterAlarm {
   const char* name() {return PSTR("Water Alarm");}
   int cardinality() {return APP_CARDINALITY_UNDEFINED;}
   void registerVars();
   void whenCreated();
   void monitor(int x,int y,int w,int h);
   void timeSlice(int fg, frequency fr);
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2);
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered();
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b);
   int evalCondition(int a, uint16_t* array) {return 0;}
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 1;}
}
namespace waterChanger {
   const char* name() {return PSTR("Water Filler");}
   int cardinality();
   void registerVars();
   void whenCreated();
   void monitor(int x,int y,int w,int h) {}
   void timeSlice(int fg, frequency fr);
   const char* actionsDef(int kind) {return 0;}
   const char* conditionsDef(int kind2) {return 0;}
   void setConditionParameter(int kind, int param) {}
   void setActionParameter(int kind, int param) {}
   void printConditionParameter(int a, int b, uint16_t c) {print(c);}
   void printActionParameter(int a, int b, uint16_t c) {print(c);}
   void whenPowered();
   void startScreen();
   void whenDestroyed() {}
   const char* getLabelForPort(int a, int b);
   int evalCondition(int a, uint16_t* array) {return 0;}
   void performAction(int a, uint16_t* array) {}
   int hasMonitor() {return 0;}
}
const char* callGeneratedNameForType(int type) {
    if (type==0) return acclimation::name();
    if (type==1) return admin::name();
    if (type==2) return aqua::name();
    if (type==3) return beeper::name();
    if (type==4) return berlinuhr::name();
    if (type==5) return button::name();
    if (type==6) return caluchitos::name();
    if (type==7) return clock::name();
    if (type==8) return debug::name();
    if (type==9) return desktop::name();
    if (type==10) return dice::name();
    if (type==11) return helloWorld::name();
    if (type==12) return led::name();
    if (type==13) return monitor::name();
    if (type==14) return ph::name();
    if (type==15) return portsManager::name();
    if (type==16) return puoooc::name();
    if (type==17) return reservoir::name();
    if (type==18) return solenoid::name();
    if (type==19) return tds::name();
    if (type==20) return th::name();
    if (type==21) return timer::name();
    if (type==22) return waterAlarm::name();
    if (type==23) return waterChanger::name();
    return 0;
}
const char* callGeneratedName(int instance) {
    return callGeneratedNameForType(typeForInstance(instance));
}
int callGeneratedCardinalityForType(int type) {
    if (type==0) return acclimation::cardinality();
    if (type==1) return admin::cardinality();
    if (type==2) return aqua::cardinality();
    if (type==3) return beeper::cardinality();
    if (type==4) return berlinuhr::cardinality();
    if (type==5) return button::cardinality();
    if (type==6) return caluchitos::cardinality();
    if (type==7) return clock::cardinality();
    if (type==8) return debug::cardinality();
    if (type==9) return desktop::cardinality();
    if (type==10) return dice::cardinality();
    if (type==11) return helloWorld::cardinality();
    if (type==12) return led::cardinality();
    if (type==13) return monitor::cardinality();
    if (type==14) return ph::cardinality();
    if (type==15) return portsManager::cardinality();
    if (type==16) return puoooc::cardinality();
    if (type==17) return reservoir::cardinality();
    if (type==18) return solenoid::cardinality();
    if (type==19) return tds::cardinality();
    if (type==20) return th::cardinality();
    if (type==21) return timer::cardinality();
    if (type==22) return waterAlarm::cardinality();
    if (type==23) return waterChanger::cardinality();
    return 0;
}
int callGeneratedCardinality(int instance) {
    return callGeneratedCardinalityForType(typeForInstance(instance));
}
void callGeneratedRegisterVarsForType(int type) {
    if (type==0) acclimation::registerVars();
    if (type==1) admin::registerVars();
    if (type==2) aqua::registerVars();
    if (type==3) beeper::registerVars();
    if (type==4) berlinuhr::registerVars();
    if (type==5) button::registerVars();
    if (type==6) caluchitos::registerVars();
    if (type==7) clock::registerVars();
    if (type==8) debug::registerVars();
    if (type==9) desktop::registerVars();
    if (type==10) dice::registerVars();
    if (type==11) helloWorld::registerVars();
    if (type==12) led::registerVars();
    if (type==13) monitor::registerVars();
    if (type==14) ph::registerVars();
    if (type==15) portsManager::registerVars();
    if (type==16) puoooc::registerVars();
    if (type==17) reservoir::registerVars();
    if (type==18) solenoid::registerVars();
    if (type==19) tds::registerVars();
    if (type==20) th::registerVars();
    if (type==21) timer::registerVars();
    if (type==22) waterAlarm::registerVars();
    if (type==23) waterChanger::registerVars();
}
void callGeneratedRegisterVars(int instance) {
     callGeneratedRegisterVarsForType(typeForInstance(instance));
}
void callGeneratedWhenCreatedForType(int type) {
    if (type==0) acclimation::whenCreated();
    if (type==1) admin::whenCreated();
    if (type==2) aqua::whenCreated();
    if (type==3) beeper::whenCreated();
    if (type==4) berlinuhr::whenCreated();
    if (type==5) button::whenCreated();
    if (type==6) caluchitos::whenCreated();
    if (type==7) clock::whenCreated();
    if (type==8) debug::whenCreated();
    if (type==9) desktop::whenCreated();
    if (type==10) dice::whenCreated();
    if (type==11) helloWorld::whenCreated();
    if (type==12) led::whenCreated();
    if (type==13) monitor::whenCreated();
    if (type==14) ph::whenCreated();
    if (type==15) portsManager::whenCreated();
    if (type==16) puoooc::whenCreated();
    if (type==17) reservoir::whenCreated();
    if (type==18) solenoid::whenCreated();
    if (type==19) tds::whenCreated();
    if (type==20) th::whenCreated();
    if (type==21) timer::whenCreated();
    if (type==22) waterAlarm::whenCreated();
    if (type==23) waterChanger::whenCreated();
}
void callGeneratedWhenCreated(int instance) {
     callGeneratedWhenCreatedForType(typeForInstance(instance));
}
void callGeneratedMonitorForType(int type,int x,int y,int w,int h) {
    if (type==0) acclimation::monitor(x,y,w,h);
    if (type==1) admin::monitor(x,y,w,h);
    if (type==2) aqua::monitor(x,y,w,h);
    if (type==3) beeper::monitor(x,y,w,h);
    if (type==4) berlinuhr::monitor(x,y,w,h);
    if (type==5) button::monitor(x,y,w,h);
    if (type==6) caluchitos::monitor(x,y,w,h);
    if (type==7) clock::monitor(x,y,w,h);
    if (type==8) debug::monitor(x,y,w,h);
    if (type==9) desktop::monitor(x,y,w,h);
    if (type==10) dice::monitor(x,y,w,h);
    if (type==11) helloWorld::monitor(x,y,w,h);
    if (type==12) led::monitor(x,y,w,h);
    if (type==13) monitor::monitor(x,y,w,h);
    if (type==14) ph::monitor(x,y,w,h);
    if (type==15) portsManager::monitor(x,y,w,h);
    if (type==16) puoooc::monitor(x,y,w,h);
    if (type==17) reservoir::monitor(x,y,w,h);
    if (type==18) solenoid::monitor(x,y,w,h);
    if (type==19) tds::monitor(x,y,w,h);
    if (type==20) th::monitor(x,y,w,h);
    if (type==21) timer::monitor(x,y,w,h);
    if (type==22) waterAlarm::monitor(x,y,w,h);
    if (type==23) waterChanger::monitor(x,y,w,h);
}
void callGeneratedMonitor(int instance,int x,int y,int w,int h) {
     callGeneratedMonitorForType(typeForInstance(instance),x,y,w,h);
}
void callGeneratedTimeSliceForType(int type,int fg, frequency fr) {
    if (type==0) acclimation::timeSlice(fg,fr);
    if (type==1) admin::timeSlice(fg,fr);
    if (type==2) aqua::timeSlice(fg,fr);
    if (type==3) beeper::timeSlice(fg,fr);
    if (type==4) berlinuhr::timeSlice(fg,fr);
    if (type==5) button::timeSlice(fg,fr);
    if (type==6) caluchitos::timeSlice(fg,fr);
    if (type==7) clock::timeSlice(fg,fr);
    if (type==8) debug::timeSlice(fg,fr);
    if (type==9) desktop::timeSlice(fg,fr);
    if (type==10) dice::timeSlice(fg,fr);
    if (type==11) helloWorld::timeSlice(fg,fr);
    if (type==12) led::timeSlice(fg,fr);
    if (type==13) monitor::timeSlice(fg,fr);
    if (type==14) ph::timeSlice(fg,fr);
    if (type==15) portsManager::timeSlice(fg,fr);
    if (type==16) puoooc::timeSlice(fg,fr);
    if (type==17) reservoir::timeSlice(fg,fr);
    if (type==18) solenoid::timeSlice(fg,fr);
    if (type==19) tds::timeSlice(fg,fr);
    if (type==20) th::timeSlice(fg,fr);
    if (type==21) timer::timeSlice(fg,fr);
    if (type==22) waterAlarm::timeSlice(fg,fr);
    if (type==23) waterChanger::timeSlice(fg,fr);
}
void callGeneratedTimeSlice(int instance,int fg, frequency fr) {
     callGeneratedTimeSliceForType(typeForInstance(instance),fg,fr);
}
const char* callGeneratedActionsDefForType(int type,int kind) {
    if (type==0) return acclimation::actionsDef(kind);
    if (type==1) return admin::actionsDef(kind);
    if (type==2) return aqua::actionsDef(kind);
    if (type==3) return beeper::actionsDef(kind);
    if (type==4) return berlinuhr::actionsDef(kind);
    if (type==5) return button::actionsDef(kind);
    if (type==6) return caluchitos::actionsDef(kind);
    if (type==7) return clock::actionsDef(kind);
    if (type==8) return debug::actionsDef(kind);
    if (type==9) return desktop::actionsDef(kind);
    if (type==10) return dice::actionsDef(kind);
    if (type==11) return helloWorld::actionsDef(kind);
    if (type==12) return led::actionsDef(kind);
    if (type==13) return monitor::actionsDef(kind);
    if (type==14) return ph::actionsDef(kind);
    if (type==15) return portsManager::actionsDef(kind);
    if (type==16) return puoooc::actionsDef(kind);
    if (type==17) return reservoir::actionsDef(kind);
    if (type==18) return solenoid::actionsDef(kind);
    if (type==19) return tds::actionsDef(kind);
    if (type==20) return th::actionsDef(kind);
    if (type==21) return timer::actionsDef(kind);
    if (type==22) return waterAlarm::actionsDef(kind);
    if (type==23) return waterChanger::actionsDef(kind);
    return 0;
}
const char* callGeneratedActionsDef(int instance,int kind) {
    return callGeneratedActionsDefForType(typeForInstance(instance),kind);
}
const char* callGeneratedConditionsDefForType(int type,int kind2) {
    if (type==0) return acclimation::conditionsDef(kind2);
    if (type==1) return admin::conditionsDef(kind2);
    if (type==2) return aqua::conditionsDef(kind2);
    if (type==3) return beeper::conditionsDef(kind2);
    if (type==4) return berlinuhr::conditionsDef(kind2);
    if (type==5) return button::conditionsDef(kind2);
    if (type==6) return caluchitos::conditionsDef(kind2);
    if (type==7) return clock::conditionsDef(kind2);
    if (type==8) return debug::conditionsDef(kind2);
    if (type==9) return desktop::conditionsDef(kind2);
    if (type==10) return dice::conditionsDef(kind2);
    if (type==11) return helloWorld::conditionsDef(kind2);
    if (type==12) return led::conditionsDef(kind2);
    if (type==13) return monitor::conditionsDef(kind2);
    if (type==14) return ph::conditionsDef(kind2);
    if (type==15) return portsManager::conditionsDef(kind2);
    if (type==16) return puoooc::conditionsDef(kind2);
    if (type==17) return reservoir::conditionsDef(kind2);
    if (type==18) return solenoid::conditionsDef(kind2);
    if (type==19) return tds::conditionsDef(kind2);
    if (type==20) return th::conditionsDef(kind2);
    if (type==21) return timer::conditionsDef(kind2);
    if (type==22) return waterAlarm::conditionsDef(kind2);
    if (type==23) return waterChanger::conditionsDef(kind2);
    return 0;
}
const char* callGeneratedConditionsDef(int instance,int kind2) {
    return callGeneratedConditionsDefForType(typeForInstance(instance),kind2);
}
void callGeneratedSetConditionParameterForType(int type,int kind, int param) {
    if (type==0) acclimation::setConditionParameter(kind,param);
    if (type==1) admin::setConditionParameter(kind,param);
    if (type==2) aqua::setConditionParameter(kind,param);
    if (type==3) beeper::setConditionParameter(kind,param);
    if (type==4) berlinuhr::setConditionParameter(kind,param);
    if (type==5) button::setConditionParameter(kind,param);
    if (type==6) caluchitos::setConditionParameter(kind,param);
    if (type==7) clock::setConditionParameter(kind,param);
    if (type==8) debug::setConditionParameter(kind,param);
    if (type==9) desktop::setConditionParameter(kind,param);
    if (type==10) dice::setConditionParameter(kind,param);
    if (type==11) helloWorld::setConditionParameter(kind,param);
    if (type==12) led::setConditionParameter(kind,param);
    if (type==13) monitor::setConditionParameter(kind,param);
    if (type==14) ph::setConditionParameter(kind,param);
    if (type==15) portsManager::setConditionParameter(kind,param);
    if (type==16) puoooc::setConditionParameter(kind,param);
    if (type==17) reservoir::setConditionParameter(kind,param);
    if (type==18) solenoid::setConditionParameter(kind,param);
    if (type==19) tds::setConditionParameter(kind,param);
    if (type==20) th::setConditionParameter(kind,param);
    if (type==21) timer::setConditionParameter(kind,param);
    if (type==22) waterAlarm::setConditionParameter(kind,param);
    if (type==23) waterChanger::setConditionParameter(kind,param);
}
void callGeneratedSetConditionParameter(int instance,int kind, int param) {
     callGeneratedSetConditionParameterForType(typeForInstance(instance),kind,param);
}
void callGeneratedSetActionParameterForType(int type,int kind, int param) {
    if (type==0) acclimation::setActionParameter(kind,param);
    if (type==1) admin::setActionParameter(kind,param);
    if (type==2) aqua::setActionParameter(kind,param);
    if (type==3) beeper::setActionParameter(kind,param);
    if (type==4) berlinuhr::setActionParameter(kind,param);
    if (type==5) button::setActionParameter(kind,param);
    if (type==6) caluchitos::setActionParameter(kind,param);
    if (type==7) clock::setActionParameter(kind,param);
    if (type==8) debug::setActionParameter(kind,param);
    if (type==9) desktop::setActionParameter(kind,param);
    if (type==10) dice::setActionParameter(kind,param);
    if (type==11) helloWorld::setActionParameter(kind,param);
    if (type==12) led::setActionParameter(kind,param);
    if (type==13) monitor::setActionParameter(kind,param);
    if (type==14) ph::setActionParameter(kind,param);
    if (type==15) portsManager::setActionParameter(kind,param);
    if (type==16) puoooc::setActionParameter(kind,param);
    if (type==17) reservoir::setActionParameter(kind,param);
    if (type==18) solenoid::setActionParameter(kind,param);
    if (type==19) tds::setActionParameter(kind,param);
    if (type==20) th::setActionParameter(kind,param);
    if (type==21) timer::setActionParameter(kind,param);
    if (type==22) waterAlarm::setActionParameter(kind,param);
    if (type==23) waterChanger::setActionParameter(kind,param);
}
void callGeneratedSetActionParameter(int instance,int kind, int param) {
     callGeneratedSetActionParameterForType(typeForInstance(instance),kind,param);
}
void callGeneratedPrintConditionParameterForType(int type,int a, int b, uint16_t c) {
    if (type==0) acclimation::printConditionParameter(a,b,c);
    if (type==1) admin::printConditionParameter(a,b,c);
    if (type==2) aqua::printConditionParameter(a,b,c);
    if (type==3) beeper::printConditionParameter(a,b,c);
    if (type==4) berlinuhr::printConditionParameter(a,b,c);
    if (type==5) button::printConditionParameter(a,b,c);
    if (type==6) caluchitos::printConditionParameter(a,b,c);
    if (type==7) clock::printConditionParameter(a,b,c);
    if (type==8) debug::printConditionParameter(a,b,c);
    if (type==9) desktop::printConditionParameter(a,b,c);
    if (type==10) dice::printConditionParameter(a,b,c);
    if (type==11) helloWorld::printConditionParameter(a,b,c);
    if (type==12) led::printConditionParameter(a,b,c);
    if (type==13) monitor::printConditionParameter(a,b,c);
    if (type==14) ph::printConditionParameter(a,b,c);
    if (type==15) portsManager::printConditionParameter(a,b,c);
    if (type==16) puoooc::printConditionParameter(a,b,c);
    if (type==17) reservoir::printConditionParameter(a,b,c);
    if (type==18) solenoid::printConditionParameter(a,b,c);
    if (type==19) tds::printConditionParameter(a,b,c);
    if (type==20) th::printConditionParameter(a,b,c);
    if (type==21) timer::printConditionParameter(a,b,c);
    if (type==22) waterAlarm::printConditionParameter(a,b,c);
    if (type==23) waterChanger::printConditionParameter(a,b,c);
}
void callGeneratedPrintConditionParameter(int instance,int a, int b, uint16_t c) {
     callGeneratedPrintConditionParameterForType(typeForInstance(instance),a,b,c);
}
void callGeneratedPrintActionParameterForType(int type,int a, int b, uint16_t c) {
    if (type==0) acclimation::printActionParameter(a,b,c);
    if (type==1) admin::printActionParameter(a,b,c);
    if (type==2) aqua::printActionParameter(a,b,c);
    if (type==3) beeper::printActionParameter(a,b,c);
    if (type==4) berlinuhr::printActionParameter(a,b,c);
    if (type==5) button::printActionParameter(a,b,c);
    if (type==6) caluchitos::printActionParameter(a,b,c);
    if (type==7) clock::printActionParameter(a,b,c);
    if (type==8) debug::printActionParameter(a,b,c);
    if (type==9) desktop::printActionParameter(a,b,c);
    if (type==10) dice::printActionParameter(a,b,c);
    if (type==11) helloWorld::printActionParameter(a,b,c);
    if (type==12) led::printActionParameter(a,b,c);
    if (type==13) monitor::printActionParameter(a,b,c);
    if (type==14) ph::printActionParameter(a,b,c);
    if (type==15) portsManager::printActionParameter(a,b,c);
    if (type==16) puoooc::printActionParameter(a,b,c);
    if (type==17) reservoir::printActionParameter(a,b,c);
    if (type==18) solenoid::printActionParameter(a,b,c);
    if (type==19) tds::printActionParameter(a,b,c);
    if (type==20) th::printActionParameter(a,b,c);
    if (type==21) timer::printActionParameter(a,b,c);
    if (type==22) waterAlarm::printActionParameter(a,b,c);
    if (type==23) waterChanger::printActionParameter(a,b,c);
}
void callGeneratedPrintActionParameter(int instance,int a, int b, uint16_t c) {
     callGeneratedPrintActionParameterForType(typeForInstance(instance),a,b,c);
}
void callGeneratedWhenPoweredForType(int type) {
    if (type==0) acclimation::whenPowered();
    if (type==1) admin::whenPowered();
    if (type==2) aqua::whenPowered();
    if (type==3) beeper::whenPowered();
    if (type==4) berlinuhr::whenPowered();
    if (type==5) button::whenPowered();
    if (type==6) caluchitos::whenPowered();
    if (type==7) clock::whenPowered();
    if (type==8) debug::whenPowered();
    if (type==9) desktop::whenPowered();
    if (type==10) dice::whenPowered();
    if (type==11) helloWorld::whenPowered();
    if (type==12) led::whenPowered();
    if (type==13) monitor::whenPowered();
    if (type==14) ph::whenPowered();
    if (type==15) portsManager::whenPowered();
    if (type==16) puoooc::whenPowered();
    if (type==17) reservoir::whenPowered();
    if (type==18) solenoid::whenPowered();
    if (type==19) tds::whenPowered();
    if (type==20) th::whenPowered();
    if (type==21) timer::whenPowered();
    if (type==22) waterAlarm::whenPowered();
    if (type==23) waterChanger::whenPowered();
}
void callGeneratedWhenPowered(int instance) {
     callGeneratedWhenPoweredForType(typeForInstance(instance));
}
screen callGeneratedStartScreenForType(int type) {
    if (type==0) return acclimation::startScreen;
    if (type==1) return admin::startScreen;
    if (type==2) return aqua::startScreen;
    if (type==3) return beeper::startScreen;
    if (type==4) return berlinuhr::startScreen;
    if (type==5) return button::startScreen;
    if (type==6) return caluchitos::startScreen;
    if (type==7) return clock::startScreen;
    if (type==8) return debug::startScreen;
    if (type==9) return desktop::startScreen;
    if (type==10) return dice::startScreen;
    if (type==11) return helloWorld::startScreen;
    if (type==12) return led::startScreen;
    if (type==13) return monitor::startScreen;
    if (type==14) return ph::startScreen;
    if (type==15) return portsManager::startScreen;
    if (type==16) return puoooc::startScreen;
    if (type==17) return reservoir::startScreen;
    if (type==18) return solenoid::startScreen;
    if (type==19) return tds::startScreen;
    if (type==20) return th::startScreen;
    if (type==21) return timer::startScreen;
    if (type==22) return waterAlarm::startScreen;
    if (type==23) return waterChanger::startScreen;
    return 0;
}
screen callGeneratedStartScreen(int instance) {
    return callGeneratedStartScreenForType(typeForInstance(instance));
}
void callGeneratedWhenDestroyedForType(int type) {
    if (type==0) acclimation::whenDestroyed();
    if (type==1) admin::whenDestroyed();
    if (type==2) aqua::whenDestroyed();
    if (type==3) beeper::whenDestroyed();
    if (type==4) berlinuhr::whenDestroyed();
    if (type==5) button::whenDestroyed();
    if (type==6) caluchitos::whenDestroyed();
    if (type==7) clock::whenDestroyed();
    if (type==8) debug::whenDestroyed();
    if (type==9) desktop::whenDestroyed();
    if (type==10) dice::whenDestroyed();
    if (type==11) helloWorld::whenDestroyed();
    if (type==12) led::whenDestroyed();
    if (type==13) monitor::whenDestroyed();
    if (type==14) ph::whenDestroyed();
    if (type==15) portsManager::whenDestroyed();
    if (type==16) puoooc::whenDestroyed();
    if (type==17) reservoir::whenDestroyed();
    if (type==18) solenoid::whenDestroyed();
    if (type==19) tds::whenDestroyed();
    if (type==20) th::whenDestroyed();
    if (type==21) timer::whenDestroyed();
    if (type==22) waterAlarm::whenDestroyed();
    if (type==23) waterChanger::whenDestroyed();
}
void callGeneratedWhenDestroyed(int instance) {
     callGeneratedWhenDestroyedForType(typeForInstance(instance));
}
const char* callGeneratedGetLabelForPortForType(int type,int a, int b) {
    if (type==0) return acclimation::getLabelForPort(a,b);
    if (type==1) return admin::getLabelForPort(a,b);
    if (type==2) return aqua::getLabelForPort(a,b);
    if (type==3) return beeper::getLabelForPort(a,b);
    if (type==4) return berlinuhr::getLabelForPort(a,b);
    if (type==5) return button::getLabelForPort(a,b);
    if (type==6) return caluchitos::getLabelForPort(a,b);
    if (type==7) return clock::getLabelForPort(a,b);
    if (type==8) return debug::getLabelForPort(a,b);
    if (type==9) return desktop::getLabelForPort(a,b);
    if (type==10) return dice::getLabelForPort(a,b);
    if (type==11) return helloWorld::getLabelForPort(a,b);
    if (type==12) return led::getLabelForPort(a,b);
    if (type==13) return monitor::getLabelForPort(a,b);
    if (type==14) return ph::getLabelForPort(a,b);
    if (type==15) return portsManager::getLabelForPort(a,b);
    if (type==16) return puoooc::getLabelForPort(a,b);
    if (type==17) return reservoir::getLabelForPort(a,b);
    if (type==18) return solenoid::getLabelForPort(a,b);
    if (type==19) return tds::getLabelForPort(a,b);
    if (type==20) return th::getLabelForPort(a,b);
    if (type==21) return timer::getLabelForPort(a,b);
    if (type==22) return waterAlarm::getLabelForPort(a,b);
    if (type==23) return waterChanger::getLabelForPort(a,b);
    return 0;
}
const char* callGeneratedGetLabelForPort(int instance,int a, int b) {
    return callGeneratedGetLabelForPortForType(typeForInstance(instance),a,b);
}
int callGeneratedEvalConditionForType(int type,int a, uint16_t* array) {
    if (type==0) return acclimation::evalCondition(a,array);
    if (type==1) return admin::evalCondition(a,array);
    if (type==2) return aqua::evalCondition(a,array);
    if (type==3) return beeper::evalCondition(a,array);
    if (type==4) return berlinuhr::evalCondition(a,array);
    if (type==5) return button::evalCondition(a,array);
    if (type==6) return caluchitos::evalCondition(a,array);
    if (type==7) return clock::evalCondition(a,array);
    if (type==8) return debug::evalCondition(a,array);
    if (type==9) return desktop::evalCondition(a,array);
    if (type==10) return dice::evalCondition(a,array);
    if (type==11) return helloWorld::evalCondition(a,array);
    if (type==12) return led::evalCondition(a,array);
    if (type==13) return monitor::evalCondition(a,array);
    if (type==14) return ph::evalCondition(a,array);
    if (type==15) return portsManager::evalCondition(a,array);
    if (type==16) return puoooc::evalCondition(a,array);
    if (type==17) return reservoir::evalCondition(a,array);
    if (type==18) return solenoid::evalCondition(a,array);
    if (type==19) return tds::evalCondition(a,array);
    if (type==20) return th::evalCondition(a,array);
    if (type==21) return timer::evalCondition(a,array);
    if (type==22) return waterAlarm::evalCondition(a,array);
    if (type==23) return waterChanger::evalCondition(a,array);
    return 0;
}
int callGeneratedEvalCondition(int instance,int a, uint16_t* array) {
    return callGeneratedEvalConditionForType(typeForInstance(instance),a,array);
}
void callGeneratedPerformActionForType(int type,int a, uint16_t* array) {
    if (type==0) acclimation::performAction(a,array);
    if (type==1) admin::performAction(a,array);
    if (type==2) aqua::performAction(a,array);
    if (type==3) beeper::performAction(a,array);
    if (type==4) berlinuhr::performAction(a,array);
    if (type==5) button::performAction(a,array);
    if (type==6) caluchitos::performAction(a,array);
    if (type==7) clock::performAction(a,array);
    if (type==8) debug::performAction(a,array);
    if (type==9) desktop::performAction(a,array);
    if (type==10) dice::performAction(a,array);
    if (type==11) helloWorld::performAction(a,array);
    if (type==12) led::performAction(a,array);
    if (type==13) monitor::performAction(a,array);
    if (type==14) ph::performAction(a,array);
    if (type==15) portsManager::performAction(a,array);
    if (type==16) puoooc::performAction(a,array);
    if (type==17) reservoir::performAction(a,array);
    if (type==18) solenoid::performAction(a,array);
    if (type==19) tds::performAction(a,array);
    if (type==20) th::performAction(a,array);
    if (type==21) timer::performAction(a,array);
    if (type==22) waterAlarm::performAction(a,array);
    if (type==23) waterChanger::performAction(a,array);
}
void callGeneratedPerformAction(int instance,int a, uint16_t* array) {
     callGeneratedPerformActionForType(typeForInstance(instance),a,array);
}
int callGeneratedHasMonitorForType(int type) {
    if (type==0) return acclimation::hasMonitor();
    if (type==1) return admin::hasMonitor();
    if (type==2) return aqua::hasMonitor();
    if (type==3) return beeper::hasMonitor();
    if (type==4) return berlinuhr::hasMonitor();
    if (type==5) return button::hasMonitor();
    if (type==6) return caluchitos::hasMonitor();
    if (type==7) return clock::hasMonitor();
    if (type==8) return debug::hasMonitor();
    if (type==9) return desktop::hasMonitor();
    if (type==10) return dice::hasMonitor();
    if (type==11) return helloWorld::hasMonitor();
    if (type==12) return led::hasMonitor();
    if (type==13) return monitor::hasMonitor();
    if (type==14) return ph::hasMonitor();
    if (type==15) return portsManager::hasMonitor();
    if (type==16) return puoooc::hasMonitor();
    if (type==17) return reservoir::hasMonitor();
    if (type==18) return solenoid::hasMonitor();
    if (type==19) return tds::hasMonitor();
    if (type==20) return th::hasMonitor();
    if (type==21) return timer::hasMonitor();
    if (type==22) return waterAlarm::hasMonitor();
    if (type==23) return waterChanger::hasMonitor();
    return 0;
}
int callGeneratedHasMonitor(int instance) {
    return callGeneratedHasMonitorForType(typeForInstance(instance));
}
