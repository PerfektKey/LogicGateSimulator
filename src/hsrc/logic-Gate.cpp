#include "../header/Logic-Gate.h"


LogicGate::LogicGate(){
    pins["input"] = std::map<std::string,logicOperandi>();
    pins["output"] = std::map<std::string,logicOperandi>();
    name = "";
}
LogicGate::~LogicGate(){
    //std::cout << "destroy gate: " << this << " -> " << name << "\n";
}

LogicGate::LogicGate(LogicGate&& other){
    logics = std::move(other.logics);
    pins = std::move(other.pins);
    outsideInputs = std::move(other.outsideInputs);

    name = other.name;
    //std::cout << name << " : " << other.name << "\n";
    color = other.color;
    return;
    other.name = std::string();
    other.color = sf::Color();
    other.logics = std::map<std::string,logicOperandi>();
    other.pins = std::map<std::string,std::map<std::string,logicOperandi>>();
    other.outsideInputs = std::map<std::string,logicOperandi*>();
}

LogicGate& LogicGate::operator=(LogicGate&& other){
        if (this != &other){        
        logics = std::move(other.logics);
        pins = std::move(other.pins);
        outsideInputs = std::move(other.outsideInputs);

        name = other.name;
        color = other.color;

        other.logics = std::map<std::string,logicOperandi>();
        other.pins = std::map<std::string,std::map<std::string,logicOperandi>>();
        other.outsideInputs = std::map<std::string,logicOperandi*>();
        }
    return *this;
}

void LogicGate::setOutput(STATE st){
    if (pins["input"].size() == 0 && pins["output"].size() == 1)
        pins["output"].at(0).output = st;
}
STATE LogicGate::getOutput(){
    if (pins["input"].size() == 0 && pins["output"].size() == 1)
        return pins["output"].at(0).output;
    if (pins["input"].size() == 1 && pins["output"].size() == 0)
        return pins["input"].at(0).output;
    return STATE::FLOATING;
}


void LogicGate::addLogicOP(logicOperandi LOP){
    if (LOP.uid == "")
        return;
    /*if (logics.find(LOP.uid) == logics.end())
        return;*/
    logics[LOP.uid] = LOP;
}
void LogicGate::addLogicOP(LOGIC_TYPE type,std::string uid){
    logics[uid] = std::move(logicOperandi(uid,type,STATE::LOW));
}
std::map<std::string,logicOperandi>& LogicGate::getLogicOP(){
    return logics;
}

//set Add Input/Output and set get Input/Output
void LogicGate::addInputPin(std::string uid){
    pins["input"][uid] = std::move(logicOperandi(uid,LOGIC_TYPE::PIN,STATE::LOW));
}
void LogicGate::addInputPin(std::string uid,std::string name){
    logicOperandi lop(uid,LOGIC_TYPE::PIN,STATE::LOW);
    lop.name = name;
    pins["input"][uid] = std::move(lop);
}
std::map<std::string,logicOperandi>& LogicGate::getInputs(){
    return pins["input"];
}

void LogicGate::addOutputPin(std::string uid){
    pins["output"][uid] = logicOperandi(uid,LOGIC_TYPE::PIN,STATE::LOW);
}
void LogicGate::addOutputPin(std::string uid,std::string name){
    logicOperandi lop(uid,LOGIC_TYPE::PIN,STATE::LOW);
    lop.name = name;
    pins["output"][uid] = lop;
}
std::map<std::string,logicOperandi>& LogicGate::getOutputs(){
    return pins["output"];
}

std::map<std::string,logicOperandi*>& LogicGate::getOInputs(){
    return outsideInputs;
}

void LogicGate::connectIO(std::string to,std::string from){//conect by searching for the LOP in the maps
    getLOP(to)->inputs.push_back(getLOP(from));
}
void LogicGate::connectIO(logicOperandi& to,logicOperandi& from){//conect by reference from two LOP's
    to.inputs.push_back(&from);
}

void LogicGate::connectGatesIO(logicOperandi& to,logicOperandi& from){
    to.inputs.push_back(&from);
}

void LogicGate::evaluate(logicOperandi& LOP, unsigned int current_cycle){
    if (LOP.evaluation_cycle == current_cycle && LOP.type != LOGIC_TYPE::PIN && LOP.type != LOGIC_TYPE::ANY){
        //std::cout << "already evaluate from: " << LOP.uid << " = " << LOP.type << "\n";
        return;
    }
    LOP.evaluation_cycle = current_cycle;
    //std::cout << "evaluate from: " << LOP.uid << " = " << LOP.type << "\n";
    switch (LOP.type)
    {
    case LOGIC_TYPE::AND://if both inputs are HIGH output HIGH
        if (LOP.inputs.size() != 2)
            break;
        evaluate(*LOP.inputs.at(0), current_cycle);
        evaluate(*LOP.inputs.at(1), current_cycle);
        LOP.output = LogicalAND(LOP.inputs.at(0)->output, LOP.inputs.at(1)->output);
        break;
    case LOGIC_TYPE::NOT://if input is HIGH output LOW (REVERSE the input)
        //std::cout << "NOTS: " << LOP.inputs.size() << "\n";
        /*for (logicOperandi* lop : LOP.inputs){
            std::cout << lop->uid << "\n";
        }*/
        if (LOP.inputs.size() != 1)
            break;
        evaluate(*LOP.inputs.at(0), current_cycle);
        LOP.output = LogicalNOT(LOP.inputs.at(0)->output);
        break;
    case LOGIC_TYPE::PIN://if any input is HIGH the LOP's output is HIGH
        if (LOP.inputs.size() == 0)
            break;
        LOP.output = STATE::LOW;
        for (const logicOperandi* lop : LOP.inputs){
            evaluate((logicOperandi&)(*lop), current_cycle);
            if (lop->output == STATE::HIGH){
                //std::cout << lop->uid << " from " << LOP.uid << " is high\n";
                LOP.output = STATE::HIGH;
                break;
            }
            //std::cout << lop->uid << " ret to " << LOP.uid << "\n";
        }
        break;
    case LOGIC_TYPE::ANY:
        if (LOP.inputs.size() == 0)
            break;
        for (const logicOperandi* lop : LOP.inputs){
            LOP.output = STATE::LOW;
            evaluate((logicOperandi&)(*lop), current_cycle);
            if (lop->output == STATE::HIGH){
                LOP.output = STATE::HIGH;
                break;
            }
        }
        break;
        
    default:
        break;
    }
}

int tmp::findchar(const std::string& string,char toFind,unsigned int occ){
    unsigned int found = 0;
    for (int i = 0;i < string.size();i++){
        if (string.at(i) != toFind)
            continue;
        found++;
        if (found == occ)
            return i;
    }
    return -1;
}

void LogicGate::simulateOWN(unsigned int& cycle,bool status_update){
    cycle = 1;
    for (auto&[uid,pin] : pins["input"]){
        pin.output = STATE::LOW;
    }
    
        simulate(cycle);
    if (status_update)
        printState(false);
    cycle++;
    if (pins["input"].size() > 1){
        for (auto&[uid,pin] : pins["input"]){
            pin.output = STATE::HIGH;
        }
    }
    for (unsigned int i = 0;i < pins["input"].size();i++)//unsigned int j = 0;j < pins["input"].size();j++
    for (auto&[uid,pin] : pins["input"]){
        pin.output = LogicalNOT(pin.output);
        simulate(cycle);
        if (status_update && (cycle != pins["input"].size()+1 || pins["input"].size() == 1))
            printState(false);
        cycle++;
    }
}
void LogicGate::simulateOWNR(unsigned int& cycle,bool status_update){
    cycle = 1;
    for (auto&[uid,pin] : outsideInputs){
        pin->output = STATE::LOW;
    }
    
        simulate(cycle);
    if (status_update)
        printStateR(false);
    cycle++;
    if (outsideInputs.size() > 1){
        for (auto&[uid,pin] : outsideInputs){
            pin->output = STATE::HIGH;
        }
    }
    for (unsigned int i = 0;i < outsideInputs.size();i++)//unsigned int j = 0;j < pins["input"].size();j++
    for (auto&[uid,pin] : outsideInputs){
        pin->output = LogicalNOT(pin->output);
        simulate(cycle);
        if (status_update && (cycle != outsideInputs.size()+1 || outsideInputs.size() == 1))
            printStateR(false);
        cycle++;
    }
}
void LogicGate::simulate(unsigned int current_cycle){
    /*for (auto&[uid, LOP] : pins["input"]){
        evaluate(LOP, current_cycle);
    }
    for (auto&[uid,LOP] : logics){
        evaluate(LOP, current_cycle);
    }
    for (auto&[uid, LOP] : pins["output"]){
        evaluate(LOP, current_cycle);
    }*/
    //current_cycle++;
    for (int i = 0;i < 5;i++)
    for (auto&[uid, LOP] : pins["output"]){
        evaluate(LOP, current_cycle+i);
    }
    
}
void LogicGate::simulateI(unsigned int current_cycle){
    //std::cout << "sim";
    for (auto&[uid, LOP] : pins["input"]){
        evaluate(LOP, current_cycle);
        //std::cout << "\n" << "input sim\n";
    }
    /*for (auto&[uid,LOP] : logics){
        evaluate(LOP, current_cycle);
    }*/
    for (auto&[uid, LOP] : pins["output"]){
        evaluate(LOP, current_cycle);
        //std::cout << "\n" << "output sim\n";
    }
}


logicOperandi* LogicGate::getLOP(std::string uid){
    std::map<std::string,logicOperandi>::iterator search;

    search = logics.find(uid);
    if (search != logics.end())
        return &logics[uid];
    search = pins["input"].find(uid);
    if (search != pins["input"].end())
        return &pins["input"][uid];
    search = pins["output"].find(uid);
    if (search != pins["output"].end())
        return &pins["output"][uid];
    return nullptr;
}

void LogicGate::printState(bool seperator){
    if (seperator)std::cout << "\n=########################################=\ngate name:" << name << "\n";
    for (auto&[uid, LOP] : pins["input"]){
        std::cout << LOP.output << " ";
    }
    std::cout << " | ";
    for (auto&[uid, LOP] : pins["output"]){
        std::cout << LOP.output << " ";
    }
    std::cout << "\n";
    if (seperator)std::cout << "=########################################=\n";
}
void LogicGate::printStateR(bool seperator){
    if (seperator)std::cout << "\n=########################################=\ngate name:" << name << "\n";
    for (auto&[uid, LOP] : outsideInputs){
        std::cout << LOP->output << " ";
    }
    std::cout << " | ";
    for (auto&[uid, LOP] : pins["output"]){
        std::cout << LOP.output << " ";
    }
    std::cout << "\n";
    if (seperator)std::cout << "=########################################=\n";
}
void LogicGate::printInfo(){
    //std::cout << "\n=########################################=\n" << "\n";
    std::cout << "gate name: " << name << "\n";
    auto CONlamda = [](logicOperandi* lop){for (logicOperandi* inp : lop->inputs){std::cout << inp->uid << ", ";}std::cout << "\n";};
    for (auto&[uid, LOP] : logics){
        std::cout << "\t" << LOP.uid << " <- "; CONlamda(&LOP);
    }
    for (auto&[uid, LOP] : pins["input"]){
        std::cout << "\t" << LOP.uid << " <- "; CONlamda(&LOP);
    }
    for (auto&[uid, LOP] : pins["output"]){
        std::cout << "\t" << LOP.uid << " <- "; CONlamda(&LOP);
    }
    //std::cout << "\n=########################################=\n";
}

void LogicGate::setName(std::string __name){
    name = __name;
}
std::string LogicGate::getName(){
    return name;
}

void LogicGate::setColor(int c[3]){
    color = sf::Color();
    color.r = c[0];
    color.g = c[1];
    color.b = c[2];
}
sf::Color LogicGate::getColor(){
    return color;
}

void LogicGate::clear(){
    name = "";
    logics = std::map<std::string,logicOperandi>();
    pins = std::map<std::string,std::map<std::string,logicOperandi>>();
}

void JsonToGate(fs::path p, LogicGate& gate, std::string Auid){
    std::ifstream file(p);

    Json::Reader reader;
    Json::Value value;

    reader.parse(file,value);

    //gate.clear();

    gate.setName(value["name"].asString());
    int c[3];
    c[0] = value["color"][0].asInt();
    c[1] = value["color"][1].asInt();
    c[2] = value["color"][2].asInt();
    gate.setColor(c);

    Json::Value LOP = value["LOP"];
    Json::Value CON = value["CON"];

    std::map<std::string,std::string> Newuid;

    for (unsigned int i = 0;i < LOP.size();i++){
        std::string type = LOP[i]["type"].asString();
        std::string name = LOP[i]["name"].asString();
        std::string uid = LOP[i]["uniqueID"].asString();

        Newuid[uid] = uid+Auid;

        uid += Auid;

        if (type == "PIN" || type == "CTR"){
            if (LOP[i]["direction"].asString() == "input"){
                gate.addInputPin(uid,name);
            }
            else if (LOP[i]["direction"].asString() == "output")
                gate.addOutputPin(uid,name);
        }
        if (type == "AND")
            gate.addLogicOP(LOGIC_TYPE::AND, uid);
        if (type == "NOT")
            gate.addLogicOP(LOGIC_TYPE::NOT, uid);
        if (type == "ANY")
            gate.addLogicOP(LOGIC_TYPE::ANY, uid);

    }
    for (unsigned int i = 0;i < CON.size();i++){

        logicOperandi* to = gate.getLOP(Newuid.at(CON[i]["uniqueID"].asString()));
        if (to == nullptr)
            return;
        for (unsigned int j = 0;j < CON[i]["connects"].size();j++){//Newuid.at()
            logicOperandi* from = gate.getLOP(Newuid.at(CON[i]["connects"][j].asString()));
            //std::cout << Newuid.at(CON[i]["connects"][j].asString()) << "\n";
            if (from == nullptr)
                return;
            to->inputs.push_back(from);
            from = nullptr;
            //std::cout << CON[i]["uniqueID"].asString() << " <- " << CON[i]["connects"][j].asString() << "\n";
            //std::cout <<Newuid.at(CON[i]["uniqueID"].asString()) << " <- " << Newuid.at(CON[i]["connects"][j].asString()) << "\n";
        }
        to = nullptr;
    }

}
/*void getALL(std::vector<logicOperandi*>& all,logicOperandi* LOP){
    for (logicOperandi* lop : LOP->inputs){
        all.push_back(lop);
        getALL(all,lop);
    }
}
void GatesToJson(LogicGate& gate, fs::path path, std::string name){
    Json::Value root;
    Json::Value data;

    root["name"] = name;

    std::vector<logicOperandi*> all;

    std::map<std::string,std::string> Newids;

    unsigned int index = 0;
    for (auto&[uid,lop] : gate.getOutputs()){
        getALL(all,&lop);
        data.clear();
        data["name"] = lop.name;
        std::string id = name;id+="-output-";id+= std::to_string(index);
        Newids[lop.uid] = name+"-output-"+std::to_string(index);
        data["uniqueID"] = id;
        data["type"] = "PIN";
        data["direction"] = "output";
        root["LOP"][index] = data;
        //root["LOP"][index]
        index++;
    }

    /*for (logicOperandi* l : all){
        std::cout << l->uid << "\n";
    }

    std::map<LOGIC_TYPE,int> amm;
    amm[LOGIC_TYPE::AND] = 0;
    amm[LOGIC_TYPE::NOT] = 0;
    amm[LOGIC_TYPE::PIN] = 0;
    amm[LOGIC_TYPE::ANY] = 0;

    for (logicOperandi* lop : all){
        data.clear();
        if (lop->type == LOGIC_TYPE::AND){
            data["uniqueID"] = name+"-and-"+std::to_string(amm.at(LOGIC_TYPE::AND));
            data["type"] = "AND";
            Newids[lop->uid] = name+"-and-"+std::to_string(amm.at(LOGIC_TYPE::AND));
            amm[LOGIC_TYPE::AND]++;
        }
        if (lop->type == LOGIC_TYPE::NOT){
            data["uniqueID"] = name+"-not-"+std::to_string(amm.at(LOGIC_TYPE::NOT));
            data["type"] = "NOT";
            Newids[lop->uid] = name+"-not-"+std::to_string(amm.at(LOGIC_TYPE::NOT));
            amm[LOGIC_TYPE::NOT]++;
        }
        if (lop->type == LOGIC_TYPE::PIN){
            if (lop->inputs.size() != 0){
                data["uniqueID"] = name+"-any-"+std::to_string(amm.at(LOGIC_TYPE::ANY));
                data["type"] = "ANY";
                Newids[lop->uid] = name+"-any-"+std::to_string(amm.at(LOGIC_TYPE::ANY));
                std::cout << lop->uid << " -> " << name+"-any-"+std::to_string(amm.at(LOGIC_TYPE::ANY)) << "\n";
                amm[LOGIC_TYPE::ANY]++;
            }else{
                data["name"] = lop->name;
                data["uniqueID"] = name+"-input-"+std::to_string(amm.at(LOGIC_TYPE::PIN));
                data["type"] = "PIN";
                data["direction"] = "input";
                Newids[lop->uid] = name+"-input-"+std::to_string(amm.at(LOGIC_TYPE::PIN));
                amm[LOGIC_TYPE::PIN]++;
            }
        }
        if (lop->type == LOGIC_TYPE::ANY){
            data["uniqueID"] = name+"-any-"+std::to_string(amm.at(LOGIC_TYPE::ANY));
            data["type"] = "ANY";
            Newids[lop->uid] = name+"-any-"+std::to_string(amm.at(LOGIC_TYPE::ANY));
            amm[LOGIC_TYPE::ANY]++;
        }
        

        root["LOP"][index] = data;
        index++;
    }
    index = 0;
    for (auto&[uid,lop] : gate.getOutputs()){
        if (lop.inputs.size() == 0)
            continue;
        data.clear();
        data["uniqueID"] = Newids.at(lop.uid);
        unsigned int j = 0;
        for (logicOperandi* lopInp : lop.inputs){
            data["connects"][j] = Newids.at(lopInp->uid);
        }
        root["CON"][index] = data;
        index++;
    }
    for (logicOperandi* lop : all){
        if (lop->inputs.size() == 0)
            continue;
        data.clear();
        data["uniqueID"] = Newids.at(lop->uid);
        unsigned int j = 0;
        for (logicOperandi* lopInp : lop->inputs){
            data["connects"][j] = Newids.at(lopInp->uid);
            std::cout << lop->uid << " = " << Newids.at(lop->uid) << "\n-> " << lopInp->uid << " = " << Newids.at(lopInp->uid) << "\n";
            j++;
        }
        root["CON"][index] = data;
        index++;
    }
    //std::cout << all << "\n";

    std::string json_file;

    Json::StreamWriterBuilder builder;
    json_file = Json::writeString(builder, root);
    //std::cout << json_file << std::endl;


    std::ofstream file;
    file.open(path, std::ios::trunc);
    file << json_file;
    //file.write()
    file.close();

}*/