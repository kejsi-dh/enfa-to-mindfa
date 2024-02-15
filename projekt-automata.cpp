#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <deque>
using namespace std;

struct automat {
    int nr_gjendjesh;
    int nr_char;
    char alfabet[100];
    int q0;
    int nr_gjendje_fundore;
    int gjendje_fundore[100];
    int kalim;
    vector<int> kalimet[100][100];
};

void te_dhenat(const automat &a) {
    cout << "\nNumri i gjendjeve: " << a.nr_gjendjesh << '\n';
    cout << "Alfabeti perbehet nga " << a.nr_char << " karaktere" <<'\n';
    cout << "Karakteret e alfabetit: ";
    for(int i=0; i<a.nr_char; i++)
        cout << a.alfabet[i] << " ";
    cout << "\nGjendja fillestare: " << a.q0 << '\n';
    cout << "Automati perbehet nga " << a.nr_gjendje_fundore << " gjendje perfundimtare: ";
    for(int i=0; i<a.nr_gjendje_fundore; i++)
        cout << a.gjendje_fundore[i] << " ";
    cout << "\nKemi " << a.kalim << " kalime: " << '\n';
    for(int i=0; i<a.nr_gjendjesh; i++){
            for(int j=0; j<a.nr_char; j++){
                cout << "Gjendja "<< i << " & Karakteri " << a.alfabet[j] << ": \n";
                for(int l : a.kalimet[i][j+1])
                    cout << l << " ";
                cout <<'\n';
            }
    }
}

int gjej_poz(const automat &a, char c) {
    for(int i=0; i<a.nr_char; i++)
        if (a.alfabet[i] == c)
            return i;
    return -1;
}

int e_final(const automat &a, int gjendje) {
    for(int i=0; i< a.nr_gjendje_fundore; i++)
        if(a.gjendje_fundore[i] == gjendje)
            return i;
    return -1;
}

int gjej_gjendjen(const int vec[], int nr, int gjendje) {
    for(int i=0; i<nr; i++)
        if(vec[i] == gjendje)
            return i;
    return -2;
}

void automati_fillestar(automat &a) {
    cout << "Jepni numrin e gjendjeve:\n";
    cin >> a.nr_gjendjesh;

    cout << "Jepni numrin e karaktereve te alfabetit:\n";
    cin >> a.nr_char;
    cout << "Jepni karakteret:\n";
    for(int i=0; i<a.nr_char; i++)
        cin >> a.alfabet[i];

    cout << "Jepni gjendjen fillestare:\n";
    cin >> a.q0;

    cout << "Jepni numrin e gjendjeve fundore:\n";
    cin >> a.nr_gjendje_fundore;
    cout << "Jepni gjendjen/gjendjet fundore:\n";
    for(int i=0; i<a.nr_gjendje_fundore; i++)
        cin >> a.gjendje_fundore[i];

    cout << "Jepni numrin e kalimeve:\n";
    cin >> a.kalim;
    cout << "Jepni kalimet ne formen [gjendje karakter gjendje]:\n";
    for(int i=0; i<a.kalim; i++){
        int p; int q; char r;
        cin >> p >> r >> q;
        int s = gjej_poz(a, r);
        a.kalimet[p][s+1].push_back(q);
    }
}

void enfa_to_nfa(automat a, automat &nfa){
    //epsilon mbyllja
    vector<int> e_closure[100];
    for(int i=0; i<a.nr_gjendjesh; i++){
        int vizituar[106];
        for(int j=0; j<=100; j++)
            vizituar[j] = 0; //markohen te gjitha gjendjet me 0
        deque<int> current;
        current.push_back(i); //shtohen gjendjet ne current
        vizituar[i] = 1; //gjendjet tashme shenohen me 1
        while(!current.empty()){
            int actual = current.front(); //mbahet gjendja e pare e current
            current.pop_front(); //hiqet nga current
            for(int j : a.kalimet[actual][0])
                if(vizituar[j] == 0){
                    vizituar[j] = 1; //nqs ka gjendje akoma te shenuar me 0 ne matricen kalimet, shenohet me 1
                    current.push_back(j); //shtohet ne fund te current
                }
        }
	//nqs gjendet nje gjendje eshte e shenuar me 1, shtohet ne array-in e espilon mbylljes
        for(int j=0; j<a.nr_gjendjesh; j++)
            if(vizituar[j] == 1)
                e_closure[i].push_back(j);
    }

    //funksioni i kalimit
    vector<int> funk_kalimit[100][100];
    for(int i=0; i<a.nr_char; i++){
        for(int j=0; j<a.nr_gjendjesh; j++){
            set<int> letters;
            for(int k : e_closure[j]){ //iterohen gjendjet qe ndodhen ne e_closure
		//iterohen kalimet midis gjendjes qe ndodhet ne e_closure dhe nje karakter nga alfabeti
                for(int l : a.kalimet[k][i+1]){
                    letters.insert(l); //shtohen gjendjet ne letters
                }
            }
            set<int> f;
            for(int x : letters){ //iterohen gjendjet qe ndodhen ne letters
		//iterohen gjendjet e e_closure ne korrespondence me kalimet ne letters
                for (int k : e_closure[x]){
                    f.insert(k); //shtohen gjendjet ne f
                }
            }
            for(auto x : f)
                funk_kalimit[j][i+1].push_back(x); //shtohen gjendjet ne funk_kalimit
            letters.clear();
            f.clear();
        }
    }

    int q01 = a.q0; //gjendja fillestare
    set<int> finale;
    for(int i=0; i<a.nr_gjendjesh; i++){
        for(unsigned int j=0; j<e_closure[i].size(); j++)
            for(int k=0; k<a.nr_gjendje_fundore; k++)
                if (e_closure[i][j] == a.gjendje_fundore[k])
                    finale.insert(i); //shtohen gjendjet fundore ne set-in finale
    }

    //eleminohen gjendjet e teperta
    int viz[106], put[106];
    for(int i=0; i<100; i++)
        viz[i] = 0; //shenohen gjendjet me 0
    for(int j=0; j<100; j++)
        put[j] = -1; //shenohen gjendjet me -1
    for(auto x : finale)
        viz[x] = 1; //gjendjet fundore shenohen me 1
    put[0] = 0; //gjendja qe ndodhet ne fillim te put shenohet me 0

    for(int i=0; i<a.nr_gjendjesh; i++){
    	if(put[i] == -1)
    		put[i] = i; //merret gjendja
    	for(int j=i+1; j<a.nr_gjendjesh; j++){
    		int ok = 0;
            if(viz[i] == viz[j]) { //nese kemi dy gjendje te njejta ne pozicione te ndryshme
            	for(int k=0; k<a.nr_char && ok == 0; k++)
			//nese madhesia e dy kalimeve te ndryshme eshte e njejte
            		if(funk_kalimit[i][k+1].size() == funk_kalimit[j][k+1].size()){
            			for(unsigned int l=0; l<funk_kalimit[i][k+1].size() && ok == 0; l++)
					//nqs totali i array-ve qe krijohen nga kjo matrice 3d eshte e ndryshme
            				if(funk_kalimit[i][k+1][l] != funk_kalimit[j][k+1][l])
            					ok = 1; //ok shenohet me 1
                        }
                        else
                        	ok = 1; //ok shenohet me 1 nese madhesia e kalimeve eshte e ndryshme
            }
            else
            	ok = 1; //ok shenohet me 1 nese ne array-in viz nuk haset asnje gjendje e njejte
            if(ok == 0)
            	put[j] = put[i]; //cdo gjendje qe vazhdon te jete e shenuar me 0, shtohet ne array-in put
        }
    }

    //zevendesohen ne funksionin e kalimit gjendjet qe do te zhduken
    set<int> eleminim;
    for(int i=0; i<a.nr_gjendjesh-1; i++)
        if(put[i] != i){ //nqs gjendja eshte e ndryshme nga gjendja e vendosur ne array-in put
            for(int j=0; j<a.nr_gjendjesh; j++)
                for(int k=0; k<a.nr_char; k++)
                    for(int &l : funk_kalimit[j][k+1]) //iterohet funksioni i kalimit
                        if(l == i)
			    //nqs gjendet nje gjendje qe nuk eshte ne aray-in put ne funk_kalimit shtohet ne put
                            l = put[i];
            eleminim.insert(i); //shtohen gjendjet qe do te hiqen ne set-in eleminim
        }

    //eleminohen gjendje te mundshme duplikate
    for(int i=0; i<a.nr_gjendjesh; i++)
        for(int j=0; j<a.nr_char; j++){
            set<int> duplicate;
            for(int l : funk_kalimit[i][j+1])
                duplicate.insert(l);
            funk_kalimit[i][j+1].clear();
            for(auto x : duplicate)
                funk_kalimit[i][j+1].push_back(x);
            duplicate.clear();
        }

    //fshihen gjendjet e teperta nga funksioni i kalimit
    int el = 0;
    for(auto x : eleminim){
    	for(int j=x-el; j<a.nr_gjendjesh; j++)
            for(int k=0; k<a.nr_char; k++){
                funk_kalimit[j][k+1].clear(); //fshihen kalimet nga funk_kalimit
                if(!funk_kalimit[j+1][k+1].empty())
                    for(unsigned int l=0; l<funk_kalimit[j+1][k+1].size(); l++)
                        funk_kalimit[j][k+1].push_back(funk_kalimit[j+1][k+1][l]);
            }
        for(int i=0; i<a.nr_gjendjesh; i++)
             for(int k=0; k<a.nr_char; k++)
                 for(unsigned int l=0; l<funk_kalimit[i][k+1].size(); l++)
                     if (funk_kalimit[i][k+1][l] >= x-el)
                         funk_kalimit[i][k+1][l]--;
        el++;
    }

    //modifikohen te dhenat
    a.nr_gjendjesh = a.nr_gjendjesh - eleminim.size();
    nfa.nr_gjendjesh = a.nr_gjendjesh;
    nfa.nr_char = a.nr_char;
    for(int i=0; i<a.nr_char; i++)
        nfa.alfabet[i] = a.alfabet[i];
    nfa.q0 = q01;

    int p = 0;

    for(auto x : finale){
    	int vf = 0;
    	for(auto y : eleminim)
    		if(y == x)
    			vf = 1;
    	if(vf == 0){
    		nfa.gjendje_fundore[p] = x;
            p++;
        }
    }
    nfa.nr_gjendje_fundore = p;
    for(auto y : eleminim)
        for(int i=0; i<nfa.nr_gjendje_fundore; i++)
                if(nfa.gjendje_fundore[i] >= y)
                    nfa.gjendje_fundore[i]--;
    nfa.kalim = 0;
    for(int i=0; i<nfa.nr_gjendjesh; i++)
        for(int j=0; j<nfa.nr_char; j++)
            for(unsigned int l=0; l<funk_kalimit[i][j+1].size(); l++){
            	nfa.kalimet[i][j+1].push_back(funk_kalimit[i][j+1][l]);
                nfa.kalim++;
            }
}

void nfa_to_dfa(automat a, automat &dfa) {
    queue < set <int> > queue;
    set < set <int> > elemente;
    queue.push({a.q0}); // @suppress("Invalid arguments")
    elemente.insert({a.q0}); // @suppress("Invalid arguments")
    vector<int> funk_kalimit[100][100];
    int lock_in_queue = 0;
    while(!queue.empty()){
        set<int> current = queue.front();
        queue.pop();
        for(int i=0; i<a.nr_char; i++){
        	set<int> actual;
            for(auto x : current){ //iterohet current qe fillimisht mban vetem gjendjen fillestare
                for(int j : a.kalimet[x][i+1]) //iterohen kalimet e gjendjes fillestare
                    actual.insert(j); //shtohen gjendjet ne actual
            }
            if(!actual.empty())
                for(auto x : actual){ //iterohet actual persa kohe qe s'eshte bosh
                    funk_kalimit[lock_in_queue][i+1].push_back(x); //shtohen gjendjet ne funk_kalimit
                }
            auto poz = elemente.find(actual); //gjen gjendjet ne actual per elementet ne set-in elemente
            if(poz == elemente.end() && *poz != actual && !actual.empty()){
                queue.push(actual); //shtohet ne queue
                elemente.insert(actual); //shtohet ne elemente
            }
        }
        lock_in_queue++;
    }

    int nr_kalimi = 0;
    for(int i=0; i<lock_in_queue; i++)
        for(int j=0; j<a.nr_char; j++){
        	set<int> verify;
            for(unsigned int x=0; x<funk_kalimit[i][j+1].size(); x++)
                verify.insert(funk_kalimit[i][j+1][x]); //shtohen kalimet ne verify
            int loc = 0;
            if(!verify.empty()){
                for(const auto &x : elemente){ //iterohet elemente
                    if(x != verify)
                        loc++; //loc inkrementohet nqs elementet e elemente jane te ndryshem nga verify
                    else
                        break; //dilet nga cikli ne te kundert
                }
                funk_kalimit[i][j+1].clear();
                funk_kalimit[i][j+1].push_back(loc); //shtohet elementi loc
                nr_kalimi++;
            }
            else
            funk_kalimit[i][j+1].clear(); //fshihet gjendja
        }

    //modifikohen te dhenat
    dfa.nr_gjendjesh = lock_in_queue;
    dfa.nr_char = a.nr_char;
    for(int i=0; i<a.nr_char; i++)
        dfa.alfabet[i] = a.alfabet[i];
    dfa.q0 = a.q0;

    set<int> finale;
    for (int i = 0; i < a.nr_gjendje_fundore; i++){
    	int count = 0;
        for(const auto &x : elemente){
            for(auto y : x)
                if(y == a.gjendje_fundore[i])
                    finale.insert(count);
            count++;
        }
    }
    dfa.nr_gjendje_fundore = finale.size();
    int p = 0;
    for (auto x : finale){
    	dfa.gjendje_fundore[p] = x;
        p++;
    }
    dfa.kalim = nr_kalimi;
    for (int i = 0; i < dfa.nr_gjendjesh; i++)
        for (int j = 0; j < dfa.nr_char; j++)
            for (unsigned int l = 0; l < funk_kalimit[i][j+1].size(); l++)
                dfa.kalimet[i][j+1].push_back(funk_kalimit[i][j+1][l]);
}

void dfa_to_mindfa(automat a, automat &dfa_minimal) {

    for(int i=0; i<a.nr_gjendjesh; i++)
        for(int j=0; j<a.nr_char; j++)
            if(a.kalimet[i][j+1].empty())
                a.kalimet[i][j+1].push_back(i);

    //matrica e ekuivalences
    int matrice_ekuivalente[100][100];
    for(int i=0; i<a.nr_gjendjesh; i++)
        for(int j=0; j<a.nr_gjendjesh; j++){
            matrice_ekuivalente[i][j] = -1; //ne matrice gjendjet e njejta shenohen me -1
            if(i>j){
		//shenohen me 1 gjendjet ku gjendja e pare eshte me e madhe se gjendja e dyte
                matrice_ekuivalente[i][j] = 1; 
                if((e_final(a,i) == -1 && e_final(a,j) != -1) || (e_final(a,j) == -1 && e_final(a,i) != -1))
                    matrice_ekuivalente[i][j] = 0; //shenohet me 0 nqs gjenden gjendje fundore te shenuara me -1
                }
        }

    int ok = 1;
    while (ok == 1){
        ok = 0;
        for(int i=0; i<a.nr_char; i++)
        	for(int j=0; j<a.nr_gjendjesh; j++)
        		for(int k=0; k<j; k++){
        			int s = a.kalimet[j][i+1][0];
        			int t = a.kalimet[k][i+1][0];
        			if ((matrice_ekuivalente[s][t] == 0 || matrice_ekuivalente[t][s] == 0) && (matrice_ekuivalente[j][k] == 1)){
        				ok = 1;
        				matrice_ekuivalente[j][k] = 0;
        				//nqs gjenden gjendje ne kalimet tek matrica ekuivalences te shenuara me 0 dhe gjendje te shenuara me 1
        				//ok behet 1 dhe gjendjet e shenuara me 1 shenohen tashme me 0
                }
            }
    }

    int grup_gjendjet[100];
    vector<int> funk_kalimit[100][100];
    int nrgj = 0;
    for(int i=0; i<a.nr_gjendjesh; i++)
        grup_gjendjet[i] = -1; //gjendjet shenohen me -1

    for(int i=0; i<a.nr_gjendjesh; i++ ){
        for(int j=0; j<i; j++)
            if(grup_gjendjet[j] != -1 && matrice_ekuivalente[i][j] == 1){
		//gjendja do te shenohet ne te njejten menyre qe eshte shenuar gjendja e ndryshme nga -1
                grup_gjendjet[i] = grup_gjendjet[j]; 
                break; //dilet nga cikli
            }
        if(grup_gjendjet[i] == -1){
            grup_gjendjet[i] = nrgj;
            nrgj++; //numri i gjendjeve inkrementohet
        }
    }

    //krijohet nje matrice si funksion kalimi
    for(int i=0; i<nrgj; i++)
        for(int j=0; j<a.nr_char; j++)
            if(funk_kalimit[i][j+1].empty()){
                funk_kalimit[i][j+1].push_back(grup_gjendjet[a.kalimet[gjej_gjendjen(grup_gjendjet, a.nr_gjendjesh, i)][j+1][0]]);
            }


    int q01;
    q01 = grup_gjendjet[a.q0]; //gjendja e re fillestare
    set<int> finale;
    for(int i=0; i<a.nr_gjendje_fundore; i++)
        finale.insert(grup_gjendjet[a.gjendje_fundore[i]]); //shtohen gjendjet fundore ne set-in finale


    set<int> eleminim;
    //eleminohen gjendjet error
    for(int i=0; i<nrgj; i++){
        int vizituar[100];
        for(int j=0; j<=nrgj; j++)
            vizituar[j] = 0; //gjendjet shenohen me 0
        deque<int> lista;
        lista.push_back(i); //shtohen gjendjet ne lista
        vizituar[i] = 1; //gjendjet tashme shenohen me 1
        while(!lista.empty()){
            int actual = lista[0]; //actual mban gjendjen e pare te lista
            lista.pop_front(); //hiqet nga lista
            for(int j=0; j<a.nr_char; j++)
                if(vizituar[funk_kalimit[actual][j+1][0]] == 0){
                    lista.push_back(funk_kalimit[actual][j+1][0]); //shtohet gjendja error ne lista
                    vizituar[funk_kalimit[actual][j+1][0]] = 1; //kemi arritur ne gjendje errori dhe shenohet me 1
                }
        }
        int ok = 0;
        for(int j=0; j<nrgj && ok==0; j++)
            for(auto x : finale)
                if(vizituar[x] == 1)
                    ok = 1;
        if(ok == 0)
            eleminim.insert(i); //shtohen gjendjet qe do te eleminohen ne set-in eleminim
    }

    //hiqen gjendjet e paarritshme nga gjendja fillestare
    deque<int> inaccessible;
    inaccessible.push_back(q01);
    int vizitat[100];
    for(int i=0; i<nrgj; i++)
        vizitat[i] = 0; //shenohen gjendjet me 0
    vizitat[q01] = 1; //gjendja fillestare shenohet me 1
    while(!inaccessible.empty()){
        int current = inaccessible[0]; //shtohet gjendja e pare e deque-s ne current
        inaccessible.pop_front(); //hiqet nga inaccessible
        for(int j=0; j<a.nr_char; j++)
            if(!funk_kalimit[current][j+1].empty() && vizitat[funk_kalimit[current][j+1][0]] == 0){
                inaccessible.push_back(funk_kalimit[current][j+1][0]); //shtohet gjendja ne inaccessible
                vizitat[funk_kalimit[current][j+1][0]] = 1; //shenohet me 1
            }

    }

    for(int i=0; i<nrgj; i++)
        if(vizitat[i] == 0)
            eleminim.insert(i); //shtohen gjendjet qe nuk arrihen nga gjendja fillestare ne eleminim

    //hiqen nga funksioni i kalimit te gjitha gjendjet e eleminuara
    for(auto x : eleminim){
        for(int i=0; i<nrgj; i++)
            for(int j=0; j<a.nr_char; j++)
                if(!funk_kalimit[i][j+1].empty() && funk_kalimit[i][j+1][0] == x){
                	funk_kalimit[i][j+1].clear(); //fshihet gjendja qe ndodhet ne eleminim
                    funk_kalimit[i][j+1].push_back(i); //modifikohet rradha
                }
    }

    int eleminuar = 0;
    for(auto x : eleminim){
        for(int i=x-eleminuar; i<nrgj-1; i++)
            for(int j=0; j<a.nr_char; j++)
                for(unsigned int l=0; l<funk_kalimit[i][j+1].size();l++){
			//fshihet gjendja qe nuk ben me pjese ne funksionin e kalimit
                	funk_kalimit[i][j+1].clear(); 
		    //shtohen gjendjet ne funksionin e kalimit
                    funk_kalimit[i][j+1].push_back(funk_kalimit[i+1][j+1][l]); 
                }
        nrgj--; //numri i gjendjeve dekrementohet
        eleminuar++; //numri i gjendjeve te eleminuara inkrementohet
    }

    //modifikohen te dhenat
    dfa_minimal.nr_gjendjesh = nrgj;
    dfa_minimal.nr_char = a.nr_char;

    for(int i=0; i<a.nr_char; i++)
        dfa_minimal.alfabet[i] = a.alfabet[i];
    dfa_minimal.q0 = q01;
    dfa_minimal.nr_gjendje_fundore = finale.size();

    int i = 0;
    for(auto x : finale){
        dfa_minimal.gjendje_fundore[i] = x;
        i++;
    }

    dfa_minimal.kalim = nrgj * dfa_minimal.nr_char;
    for(int i=0; i<dfa_minimal.nr_gjendjesh; i++)
    	for(int j=0; j<a.nr_char; j++)
    		for(unsigned int l=0; l<funk_kalimit[i][j+1].size(); l++)
    			dfa_minimal.kalimet[i][j+1].push_back(funk_kalimit[i][j+1][l]);
}

int main() {
    automat aut, nfa, dfa, minimal;
    automati_fillestar(aut);
    enfa_to_nfa(aut, nfa); //e-afjd ne afjd
    nfa_to_dfa(nfa, dfa); //afjd ne afd
    dfa_to_mindfa(dfa, minimal); //afd ne afd minimal
    //shfaqen te dhenat per 3 automatet
    te_dhenat(nfa);
    te_dhenat(dfa);
    te_dhenat(minimal);

    return 0;
}
