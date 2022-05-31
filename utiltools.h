#ifndef UTILTOOLS_H
#define UTILTOOLS_H

#include "Preprocess.h"
// ljs add
class uitlTools {
public:
    DataFlow *Predatamem;
    vector<char *> inputarg;
    int i;
    char *funcname;
    V3DPluginArgList pluginInputList, pluginOutputList;
    V3DPluginArgItem Inputimg, InputParam, Outputimg;

    vector<char*> pluginInputArgList;
    vector<Image4DSimple *> inputimg;
    vector<Image4DSimple *> outputimg;
    Image4DSimple *otimg;


    uitlTools(DataFlow *Predatamem,vector<char *> inputarg,int i,char *funcname){
        pluginInputArgList.push_back("0");
        pluginInputArgList.push_back("0");
        for(int j=0;j<inputarg.size();j++){
            pluginInputArgList.push_back(inputarg[j]);
        }              //input imgs
        inputimg.push_back(Predatamem->outputimg[i]);
        otimg=new Image4DSimple();
        outputimg.push_back(otimg);
        Inputimg.type="Inputimg";
        Inputimg.p = (void*)(&inputimg);

        InputParam.type="InputParam";
        InputParam.p = (void*)(&pluginInputArgList);

        Outputimg.type="Outputimg";
        Outputimg.p = (void*)(&outputimg);

        pluginInputList.push_back(Inputimg);
        pluginInputList.push_back(InputParam);
        pluginOutputList.push_back(Outputimg);
    }

};








#endif // UTILTOOLS_H
