//ljs add
#include "PreLibrary.h"

void preLibrary::processcmd(const V3DPluginArgList &input, V3DPluginArgList &output)
{
    vector<char *> paras=(*(vector<char*> *)(input.at(1).p));               //input.at(1)
    inputway=QString(paras[0]);
    if(inputway!="R"&&inputway!="T"){
        qDebug()<<"Wrong output parameter! Please input 'R' or 'T'.";
        return;
    }
    if(inputway=="R"){
        qDebug()<<"The dataflow will be loaded from your Raw local folder.";
    }else if(inputway=="T"){
        qDebug()<<"The dataflow will be loaded from your Terafly folder.";
    }

    outresult=QString(paras[1]);
    if(outresult!="img"&&outresult!="swc"&&outresult!="all"){
        qDebug()<<"Wrong output format! Please input 'img' or 'swc' or 'all'.";
        return;
    }
    qDebug()<<"Your output will be "+outresult+" format.";

    if(inputway=="R"){
        inputfile=((vector<char*> *)(input.at(0).p))->at(0);                    //input.at(0)
        qinputfile=QString(inputfile);
        inputimglist=getImgNames(qinputfile);
        inputswclist=getSwcNames(qinputfile);
        int count=inputimglist.size()>inputswclist.size()?inputimglist.size():inputswclist.size();
        //qDebug()<<"count";
        datamem->dataname=inputimglist.size()>inputswclist.size()?inputimglist:inputswclist;
        for(int i=0;i<count;i++){
            Image4DSimple *nimg=new Image4DSimple();
            NeuronTree *nswc=new NeuronTree();
            datamem->push_img(nimg);
            datamem->push_swc(nswc);
        }
        //qDebug()<<"count";
        for(int i=0;i<inputimglist.size();i++){
            Image4DSimple *nimg;

            QByteArray ba1=(qinputfile+"\\"+inputimglist[i]).toLatin1(); //+"\\"+inputimglist[i]
            char *inputimg=ba1.data();

            char inimage[200];
            for(int i=0;i<200;i++){
                inimage[i]=inputimg[i];
                if(inputimg[i]=='\0')
                    break;
            }

            nimg=cb->loadImage(inimage);

            datamem->outputimg[i]=nimg;

        }
        //qDebug()<<"count";
        for(int i=0;i<inputswclist.size();i++){
            NeuronTree *nswc=new NeuronTree();
            QString swcfile=qinputfile+"\\"+inputswclist[i];
            //qDebug()<<swcfile;
            *nswc=readSWC_file(swcfile);

            datamem->outputswc[i]=nswc;

        }
        qDebug()<<"count";
        outputfile=((vector<char*> *)(output.at(0).p))->at(0);                  //output.at(0)

        vector<char *> funcparas;

        for(int i=2;i<paras.size();i++){
            if(paras[i][0]>='a'&&paras[i][0]<='z'){         //DataFlowArg[0] is empty.
                DataFlowArg.push_back(funcparas);
                funcparas.clear();
                funcparas.push_back(paras[i]);
                if(i==paras.size()-1)
                    DataFlowArg.push_back(funcparas);
                continue;
            }
            funcparas.push_back(paras[i]);
            if(i==paras.size()-1)
                DataFlowArg.push_back(funcparas);
        }
        qDebug()<<DataFlowArg.size()-1;
    }else{
        qDebug()<<"Your first func must be 'cropTerafly'!";
        inputfile=((vector<char*> *)(input.at(0).p))->at(0);                    //input.at(0)
        //qinputfile=QString(inputfile);
        outputfile=((vector<char*> *)(output.at(0).p))->at(0);
        vector<char *> funcparas;
        for(int i=2;i<paras.size();i++){
            if(paras[i][0]>='a'&&paras[i][0]<='z'){         //DataFlowArg[0] is empty.
                DataFlowArg.push_back(funcparas);
                funcparas.clear();
                funcparas.push_back(paras[i]);
                if(i==paras.size()-1)
                    DataFlowArg.push_back(funcparas);
                continue;
            }
            funcparas.push_back(paras[i]);
            if(i==paras.size()-1)
                DataFlowArg.push_back(funcparas);
        }
        qDebug()<<DataFlowArg.size()-1;
    }
}

void preLibrary::initmap()
{
    fnametodll["gf"]="gaussianfilter1.dll";
    fnametodll["app2"]="vn21.dll";
    fnametodll["im_sigma_correction"]="imPreProcess1.dll";
    fnametodll["im_subtract_minimum"]="imPreProcess1.dll";
    fnametodll["im_bilateral_filter"]="imPreProcess1.dll";
    fnametodll["im_fft_filter"]="imPreProcess1.dll";
    fnametodll["im_grey_morph"]="imPreProcess1.dll";
    fnametodll["im_enhancement"]="imPreProcess1.dll";
    fnametodll["gsdt"]="gsdt1.dll";
    fnametodll["cropTerafly"]="cropped3DImageSeries1.dll";
    fnametodll["he"]="HistogramEqualization1.dll";
    fnametodll["standardize"]="standardize_image1.dll";
    fnametodll["dtc"]="datatypeconvert1.dll";
    fnametodll["resample_swc"]="resample_swc1.dll";
    fnametodll["sort_swc"]="sort_neuron_swc1.dll";

    dlltomode["gaussianfilter1.dll"]="Preprocess";
    dlltomode["imPreProcess1.dll"]="Preprocess";
    dlltomode["cropped3DImageSeries1.dll"]="Preprocess";
    dlltomode["gsdt1.dll"]="Preprocess";
    dlltomode["HistogramEqualization1.dll"]="Preprocess";
    dlltomode["standardize_image1.dll"]="Preprocess";
    dlltomode["datatypeconvert1.dll"]="Preprocess";
    dlltomode["vn21.dll"]="Computation";
    dlltomode["resample_swc1.dll"]="Postprocess";
    dlltomode["sort_neuron_swc1.dll"]="Postprocess";
}

QString preLibrary::finddll(char *funcname)
{
    return fnametodll[funcname];
}

void preLibrary::saveimgresult(DataFlow *data, int i)
{
    QString path=QString(outputfile)+"\\"+datamem->dataname[i]+"_result.tiff";
    QByteArray ba1=(path).toLatin1();
    char *otimg=ba1.data();
    //qDebug()<<inputimg;
    char outpath[200];
    for(int j=0;j<200;j++){
        outpath[j]=otimg[j];
        if(otimg[j]=='\0')
            break;
    }
    qDebug()<<"outpath is "<<outpath;
    V3DLONG  in_sz[4];
    in_sz[0] = data->outputimg[i]->getXDim();
    in_sz[1] = data->outputimg[i]->getYDim();
    in_sz[2] = data->outputimg[i]->getZDim();
    in_sz[3] = data->outputimg[i]->getCDim();
    simple_saveimage_wrapper(*cb,outpath,data->outputimg[i]->getRawData(),in_sz,1);
//    qDebug()<<data->outputimg[i];
//    mcallback->saveImage(data->outputimg[i],(char*)outpath);
    qDebug()<<"the "<<i<<" img saved.";
}

void preLibrary::saveswcresult(DataFlow *data, int i)
{
    QString path=QString(outputfile)+"\\"+datamem->dataname[i]+"_result.swc";
    QStringList infostring;
    infostring.push_back("##Assembly line");
    infostring.push_back("##Output by superplugin");
    qDebug()<<writeSWC_file(path,*data->outputswc[i]);
}

void preLibrary::gaussfilter()
{
    for(int i=1;i<DataFlowArg.size();i++){
        vector<char*> pluginInputArgList;
        pluginInputArgList.clear();
        for(int j=1;j<DataFlowArg[i].size();j++){
            pluginInputArgList.push_back(DataFlowArg[i][j]);
        }
        for(int j=0;j<datamem->getimg_cnt();j++){
            qDebug()<<"Executing NO. "<<i<<" func.";
            pre->gaussfilter(datamem,pluginInputArgList,j,DataFlowArg[i][0]);
        }
    }

    for(int j=0;j<datamem->getimg_cnt();j++){
        qDebug()<<"outresult is "<<outresult;
        if(outresult=="img"){
            qDebug()<<"saving img!";
            saveimgresult(datamem,j);
        }
        else if(outresult=="swc"){
            qDebug()<<"saving swc!";
            saveswcresult(datamem,j);
        }else if(outresult=="all"){
            qDebug()<<"saving all!";
            qDebug()<<"Please make sure you had used both img and swc!";
            saveimgresult(datamem,j);
            saveswcresult(datamem,j);
        }
    }

}


void preLibrary::laplacianfilter()
{
    for(int i=1;i<DataFlowArg.size();i++){
        vector<char*> pluginInputArgList;
        pluginInputArgList.clear();
        for(int j=1;j<DataFlowArg[i].size();j++){
            pluginInputArgList.push_back(DataFlowArg[i][j]);
        }
        for(int j=0;j<datamem->getimg_cnt();j++){
            qDebug()<<"Executing NO. "<<i<<" func.";
            pre->laplacianfilter(datamem,pluginInputArgList,j,DataFlowArg[i][0]);
        }
    }

    for(int j=0;j<datamem->getimg_cnt();j++){
        qDebug()<<"outresult is "<<outresult;
        if(outresult=="img"){
            qDebug()<<"saving img!";
            saveimgresult(datamem,j);
        }
        else if(outresult=="swc"){
            qDebug()<<"saving swc!";
            saveswcresult(datamem,j);
        }else if(outresult=="all"){
            qDebug()<<"saving all!";
            qDebug()<<"Please make sure you had used both img and swc!";
            saveimgresult(datamem,j);
            saveswcresult(datamem,j);
        }
    }

}

void preLibrary::minmaxfilter()
{
    for(int i=1;i<DataFlowArg.size();i++){
        vector<char*> pluginInputArgList;
        pluginInputArgList.clear();
        for(int j=1;j<DataFlowArg[i].size();j++){
            pluginInputArgList.push_back(DataFlowArg[i][j]);
        }
        for(int j=0;j<datamem->getimg_cnt();j++){
            qDebug()<<"Executing NO. "<<i<<" func.";
            pre->minmaxfilter(datamem,pluginInputArgList,j,DataFlowArg[i][0]);
        }
    }

    for(int j=0;j<datamem->getimg_cnt();j++){
        qDebug()<<"outresult is "<<outresult;
        if(outresult=="img"){
            qDebug()<<"saving img!";
            saveimgresult(datamem,j);
        }
        else if(outresult=="swc"){
            qDebug()<<"saving swc!";
            saveswcresult(datamem,j);
        }else if(outresult=="all"){
            qDebug()<<"saving all!";
            qDebug()<<"Please make sure you had used both img and swc!";
            saveimgresult(datamem,j);
            saveswcresult(datamem,j);
        }
    }
}

void preLibrary::medianfilter()
{
    for(int i=1;i<DataFlowArg.size();i++){
        vector<char*> pluginInputArgList;
        pluginInputArgList.clear();
        for(int j=1;j<DataFlowArg[i].size();j++){
            pluginInputArgList.push_back(DataFlowArg[i][j]);
        }
        for(int j=0;j<datamem->getimg_cnt();j++){
            qDebug()<<"Executing NO. "<<i<<" func.";
            pre->medianfilter(datamem,pluginInputArgList,j,DataFlowArg[i][0]);
        }
    }

    for(int j=0;j<datamem->getimg_cnt();j++){
        qDebug()<<"outresult is "<<outresult;
        if(outresult=="img"){
            qDebug()<<"saving img!";
            saveimgresult(datamem,j);
        }
        else if(outresult=="swc"){
            qDebug()<<"saving swc!";
            saveswcresult(datamem,j);
        }else if(outresult=="all"){
            qDebug()<<"saving all!";
            qDebug()<<"Please make sure you had used both img and swc!";
            saveimgresult(datamem,j);
            saveswcresult(datamem,j);
        }
    }
}
