#ifndef PRELIBRARY_H
#define PRELIBRARY_H

#include "Preprocess.h"
#include "superplugin_ui.h"
#include "v3d_interface.h"

//ljs add
class preLibrary {

    explicit preLibrary(V3DPluginCallback2 &cb,const V3DPluginArgList & input, V3DPluginArgList & output) {
        this->cb = &cb;
        pre = new Preprocess(this->cb);
        datamem=new DataFlow();
        initmap();
        processcmd(input,output);
    }

    //初始化
    void processcmd(const V3DPluginArgList & input, V3DPluginArgList & output);
    void initmap();
    QString finddll(char * funcname);
    void saveimgresult(DataFlow* data,int i);
    void saveswcresult(DataFlow* data,int i);




    void gaussfilter();
    void laplacianfilter();
    void minmaxfilter();
    void medianfilter();


private:
    V3DPluginCallback2 *cb;
    Preprocess *pre;

    char * inputfile;           // can be optimized
//    char * inputapo;
    char * outputfile;
    vector<vector<char *>> DataFlowArg;
    QString qinputfile;
    QStringList inputimglist;
    QStringList inputswclist;
    QString inputway;
    QString outresult;

    unordered_map<QString,QString> fnametodll;
    unordered_map<QString,QString> dlltomode;
    //unordered_map<QString,QString> dlltofunc;

    QHBoxLayout *hlayout;
    QVBoxLayout *vlayout;

    QString datapath;

    QComboBox *preprocess;
    QPushButton *confirm;

    DataFlow *datamem;

    Preprocess *preproc;
};




















#endif // PRELIBRARY_H
