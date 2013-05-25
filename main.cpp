#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include "libr.h"

using namespace std;

Graph<string, int> make_read(map<string, string> & cmd){
    FILE* fp = fopen("myMakefile", "r");
    if (fp == NULL) {
        throw ("File is not found");
    }
    Graph<string, int> A;
    map <string, int> vtx_id;
    int id = 1;
    char c = '\0';
    while ( (c = getc(fp))!= EOF){
        string main_purpose;
        if (c!='\n' && c!= ' ') main_purpose.push_back(c);
        while ((c=getc(fp)) != ':' && c != EOF){
             main_purpose.push_back(c);
        }
        cout<<main_purpose<<" \n";
        Vetex<string> vtx;
        vtx.value = main_purpose;
        Edge<int> edg;
        pair<map <string, int>::iterator, bool> inserting (vtx_id.insert(pair<string, int> (main_purpose, id)));
        if (inserting.second == true) {
            vtx.id = id;
            ++id;
            vtx.value = main_purpose;
            A.add_vtx(vtx);
        }
        edg.first_v = (*inserting.first).second;
        while ((c = getc(fp)) != '\t'){
            string purpose;
            if (c == ' ') while ((c = getc(fp))==' ');
            while (c != ' ' && c != '\n' && c != EOF){
                purpose.push_back(c);
                fscanf(fp, "%c", &c);
            }
            if (!purpose.empty()){
               cout <<"purpose : "<<purpose<<endl;
               Vetex<string> vtx;
               pair<map <string, int>::iterator, bool> inserting (vtx_id.insert(pair<string, int> (purpose, id)));
               if (inserting.second == true) {
                    vtx.id = id;
                    ++id;
                    vtx.value = purpose;
                    A.add_vtx(vtx);
               }
               edg.second_v = (*inserting.first).second;
               Edge<int> _edg(edg.first_v, edg.second_v, 0);
               A.add_edg(_edg);
               cout << _edg;
            }
        }
        string comand;
            while ((c = getc(fp)) != '\n' && c != EOF) {
                fprintf(fp, "%c", &c);
                comand.push_back(c);
            }
        cout <<"com: "<< comand<<endl;
        cmd.insert(std::pair<string, string>(main_purpose, comand));
    }
return A;
}

int main()
{
    Graph<string, int> A;
    map <string, string> cmd;
    try {
        A = make_read(cmd);
    } catch (const char * &exp){
        cerr<<exp<<endl;
        return 0;
    }
    Graph<string, int>::iterator_DFS itr(&A);
    cout << A;
    return 0;
}
