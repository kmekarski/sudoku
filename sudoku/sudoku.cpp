#include "sudoku.h"

int sudoku::getCell(int a) { return tab[a]; }
int sudoku::getSize() { return size; }
int sudoku::getLevel() { return level; }
int sudoku::getLast_hint() { return last_hint; }
void sudoku::setSize(int size) { this->size = size; }
void sudoku::setLevel(int level) { this->level = level; }
void sudoku::reset_hint() { was_hint=false; }
bool sudoku::wasHint() { return was_hint; }
void sudoku::setLast_hint(int last_hint) { this->last_hint = last_hint; }
bool sudoku::noSolution() { return no_solution; }
void sudoku::setNoSolution(bool a) { no_solution = a; }
void sudoku::importCell(int index, int n, int size)
{
    tab.resize(size * size);
    solved.resize(size * size);
    vis.resize(size * size);
    tab[index]=n;
    if (tab[index]) {
        prefilled.push_back(true);
        fixed.push_back(true);
    }
    else {
        prefilled.push_back(false);
        fixed.push_back(false);
    }
    for (int j = 1; j <= size; j++)
        vis[index].push_back(false);
    vis[index].push_back(false);
}
bool sudoku::wasPrefilled(int index) { return prefilled[index]; }
void sudoku::insert(int w, int k, int n)
{
    tab[size*w+k]=n;
    fixed[size*w+k]=true;
    filled.push_back(size*w+k);
}
void sudoku::remove()
{
    if(filled.size())
    {
        tab[filled.back()]=0;
        solved[filled.back()]=0;
        fixed[filled.back()]=false;
        filled.pop_back();
    }
}
bool sudoku::emptyCell(int &a)
{
    for(int i=0; i<size*size; i++)
        if(solved[i]==0)
        {
            a=i;
            return true;
        }
    return false;
}
bool sudoku::possibleDigit(int a, int n)
{
    for(int i=a/size*size; i<a/size*size+size; i++)
        if(solved[i]==n) return false;
    for(int i=a%size; i<size*size; i+=size)
        if(solved[i]==n) return false;
    int x=(a/size)/int(sqrt(size))*int(sqrt(size));
    int y=(a%size)/int(sqrt(size))*int(sqrt(size));
    for(int i=x; i<x+int(sqrt(size)); i++)
        for(int j=y; j<y+int(sqrt(size)); j++)
            if(solved[size*i+j]==n) return false;
    return true;
}
bool sudoku::done()
{
    for(int i=0; i<size*size; i++)
        if(tab[i]==0) return false;
        return true;
}
bool sudoku::dfs(int a)
{
    for(int i=1; i<=size; i++)
        if(possibleDigit(a,i) && !vis[a][i])
        {
            vis[a][i]=true;
            solved[a]=i;
            int n;
            if(!emptyCell(n))
            {
                if(good()) return true;
                else
                {
                    setNoSolution(true);
                    return false;
                }
            }
            return dfs(n);
        }
    int k=1;
    while(fixed[a-k])
    {
        k++;
        if(a-k<0)
        {
            setNoSolution(true);
            return false;
        }
    }
    solved[a-k]=0;
    for(int i=a-k+1; i<size*size; i++)
        for(int j=1; j<=size; j++)
            vis[i][j]=false;
    return dfs(a-k);
}
void sudoku::solve() {
    for (int i = 0; i < size * size; i++) {
        solved[i] = tab[i];
        if (solved[i])
            fixed[i] = true;
        else
            fixed[i] = false;
        for (int j = 1; j <= size; j++) {
            vis[i][j] = false;
        }
    }
    int a;
    if(emptyCell(a))
    {
        dfs(a);
    }
}
bool sudoku::good()
{
    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            int index=size*i+j;
            int count=0;
            for(int k=i*size; k<i*size+size; k++)
            {
                if(solved[index]==solved[k]) count++;
            }
            if(count>1) return false;
            count=0;
            for(int l=j; l<size*size; l+=size)
            {
                if(solved[index]==solved[l]) count++;
            }
            if(count>1) return false;
            count=0;
            int x=(index/size)/int(sqrt(size))*int(sqrt(size));
            int y=(index%size)/int(sqrt(size))*int(sqrt(size));
            for(int m=x; m<x+int(sqrt(size)); m++)
                for(int n=y; n<y+int(sqrt(size)); n++)
                    if(solved[size*m+n]==solved[index]) count++;
            if(count>1) return false;
        }
    }
    return true;
}
bool sudoku::win()
{
    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            int index=size*i+j;
            int count=0;
            for(int k=i*size; k<i*size+size; k++)
            {
                if(tab[index]==tab[k]) count++;
            }
            if(count>1) return false;
            count=0;
            for(int l=j; l<size*size; l+=size)
            {
                if(tab[index]==tab[l]) count++;
            }
            if(count>1) return false;
            count=0;
            int x=(index/size)/int(sqrt(size))*int(sqrt(size));
            int y=(index%size)/int(sqrt(size))*int(sqrt(size));
            for(int m=x; m<x+int(sqrt(size)); m++)
                for(int n=y; n<y+int(sqrt(size)); n++)
                    if(tab[size*m+n]==tab[index]) count++;
            if(count>1) return false;
        }
    }
    return true;
}
void sudoku::hint()
{
    solve();
    if(!noSolution()) {
        srand((unsigned) time(0));
        int a = rand() % (size * size);
        while (fixed[a])
            a = rand() % (size * size);
        tab[a] = solved[a];
        fixed[a] = true;
        filled.push_back(a);
        setLast_hint(a);
        was_hint = true;
    }
    else
    {
        string level_name;
        switch (getLevel()) {
            case 1: {
                level_name = "LATWY";
            }
                break;
            case 2: {
                level_name = "SREDNI";
            }
                break;
            case 3: {
                level_name = "TRUDNY";
            }
                break;
        }
        system("cls");
        if (getLevel() > 3) cout << "SUDOKU " <<dec<< getSize() << "x" <<dec<< getSize() << " - NIESTANDARDOWY\n\n";
        else cout << "SUDOKU " <<dec<< getSize() << "x" <<dec<< getSize() << " - " << level_name << "\n\n";
        show();
        cout << "NIE ISTNIEJE ROZWIAZANIE. SPROBUJ PONOWNIE";
        Sleep(3000);
        remove();
    }
    setNoSolution(false);
}

void sudoku4x4::show()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
    cout<<"  | ";
    for(int i=1; i<=getSize(); i++)
    cout<<i<<" ";
    cout<<"\n--|---------\n";
    for(int i=0; i<getSize(); i++)
    {
        cout<<i+1<<" | ";
        for(int j = 0; j<getSize(); j++) {
            if(done() && win()) SetConsoleTextAttribute(hConsole, 2);
            else if (wasHint() && getSize() * i + j == getLast_hint()) SetConsoleTextAttribute(hConsole, 4);
            else if (wasPrefilled(getSize()*i+j)) SetConsoleTextAttribute(hConsole, 5);
            else {
                if (i >= 0 && i <= 1 && j >= 0 && j <= 1)
                    SetConsoleTextAttribute(hConsole, 6);
                else if (i >= 0 && i <= 1 && j >= 2 && j <= 3)
                    SetConsoleTextAttribute(hConsole, 7);
                else if (i >= 2 && i <= 3 && j >= 0 && j <= 1)
                    SetConsoleTextAttribute(hConsole, 7);
                else if (i >= 2 && i <= 3 && j >= 2 && j <= 3)
                    SetConsoleTextAttribute(hConsole, 6);
            }
                if (getCell(getSize() * i + j)) cout << getCell(getSize() * i + j) << " ";
                else cout << "* ";
                SetConsoleTextAttribute(hConsole, 7);
        }
        cout<<"\n";
    }
    cout<<"\n";
}
void sudoku4x4::getFile(int level)
{
    setSize(4);
    setLevel(level);
    string fname = "4x4";
    switch (level) {
        case 1: {
            fname += "easy";
        }
            break;
        case 2: {
            fname += "medium";
        }
            break;
        case 3: {
            fname += "hard";
        }
            break;
    }
    fname+=".txt";
    tab.resize(getSize() * getSize());
    solved.resize(getSize() * getSize());
    vis.resize(getSize() * getSize());
    ifstream input(fname);
    if (input) {
        for (int i = 0; i < getSize() * getSize(); i++) {
            input >> tab[i];
            if (tab[i]) {
                prefilled.push_back(true);
                fixed.push_back(true);
            }
            else {
                prefilled.push_back(false);
                fixed.push_back(false);
            }
            for (int j = 1; j <= getSize(); j++)
                vis[i].push_back(false);
            vis[i].push_back(false);
        }
        input.close();
    }
}
void sudoku9x9::show()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
    cout<<"  | ";
    for(int i=1; i<=getSize(); i++)
        cout<<i<<" ";
    cout<<"\n--|------------------\n";
    for(int i=0; i<getSize(); i++)
    {
        cout<<i+1<<" | ";
        for(int j=0; j<getSize(); j++)
        {
            if(done() && win()) SetConsoleTextAttribute(hConsole, 2);
            else if (wasHint() && getSize() * i + j == getLast_hint()) SetConsoleTextAttribute(hConsole, 4);
            else if (wasPrefilled(getSize()*i+j)) SetConsoleTextAttribute(hConsole, 5);
            else {
                if (i >= 0 && i <= 2 && j >= 0 && j <= 2)
                    SetConsoleTextAttribute(hConsole, 6);
                else if (i >= 0 && i <= 2 && j >= 3 && j <= 5)
                    SetConsoleTextAttribute(hConsole, 7);
                else if (i >= 0 && i <= 2 && j >= 6 && j <= 8)
                    SetConsoleTextAttribute(hConsole, 6);
                else if (i >= 3 && i <= 5 && j >= 0 && j <= 2)
                    SetConsoleTextAttribute(hConsole, 7);
                else if (i >= 3 && i <= 5 && j >= 3 && j <= 5)
                    SetConsoleTextAttribute(hConsole, 6);
                else if (i >= 3 && i <= 5 && j >= 6 && j <= 8)
                    SetConsoleTextAttribute(hConsole, 7);
                else if (i >= 6 && i <= 8 && j >= 0 && j <= 2)
                    SetConsoleTextAttribute(hConsole, 6);
                else if (i >= 6 && i <= 8 && j >= 3 && j <= 5)
                    SetConsoleTextAttribute(hConsole, 7);
                else if (i >= 6 && i <= 8 && j >= 6 && j <= 8)
                    SetConsoleTextAttribute(hConsole, 6);
            }
            if(getCell(getSize()*i+j)) cout<<getCell(getSize()*i+j)<<" ";
            else cout<<"* ";
            SetConsoleTextAttribute(hConsole, 7);
        }
        cout<<"\n";
    }
    cout<<"\n";
}
void sudoku9x9::getFile(int level)
{
    setSize(9);
    setLevel(level);
    string fname = "9x9";
    switch (level) {
        case 1: {
            fname += "easy";
        }
            break;
        case 2: {
            fname += "medium";
        }
            break;
        case 3: {
            fname += "hard";
        }
            break;
    }
    fname+=".txt";
    tab.resize(getSize() * getSize());
    solved.resize(getSize() * getSize());
    vis.resize(getSize()*getSize());
    ifstream input(fname);
    if (input) {
        for (int i = 0; i < getSize() * getSize(); i++) {
            input >> tab[i];
            if (tab[i]) {
                prefilled.push_back(true);
                fixed.push_back(true);
            }
            else {
                prefilled.push_back(false);
                fixed.push_back(false);
            }
            for (int j = 1; j <= getSize(); j++)
                vis[i].push_back(false);
            vis[i].push_back(false);
        }
        input.close();
    }
}
void sudoku16x16::show()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
    cout<<"  | ";
    for(int i=1; i<getSize(); i++)
        cout<<uppercase<<hex<<i<<" ";
    cout<<"G\n--|--------------------------------\n";
    for(int i=0; i<getSize(); i++)
    {
        if(i==15) cout<<"G | ";
        else cout<<i+1<<" | ";
        for(int j=0; j<getSize(); j++)
        {
            if(done() && win()) SetConsoleTextAttribute(hConsole, 2);
            else if (wasHint() && getSize() * i + j == getLast_hint()) SetConsoleTextAttribute(hConsole, 4);
            else if (wasPrefilled(getSize()*i+j)) SetConsoleTextAttribute(hConsole, 5);
            else {
                if (i >= 0 && i <= 3 && j >= 0 && j <= 3)
                    SetConsoleTextAttribute(hConsole, 6);
                else if (i >= 0 && i <= 3 && j >= 4 && j <= 7)
                    SetConsoleTextAttribute(hConsole, 7);
                else if (i >= 0 && i <= 3 && j >= 8 && j <= 11)
                    SetConsoleTextAttribute(hConsole, 6);
                else if (i >= 0 && i <= 3 && j >= 12 && j <= 15)
                    SetConsoleTextAttribute(hConsole, 7);
                else if (i >= 4 && i <= 7 && j >= 0 && j <= 3)
                    SetConsoleTextAttribute(hConsole, 7);
                else if (i >= 4 && i <= 7 && j >= 4 && j <= 7)
                    SetConsoleTextAttribute(hConsole, 6);
                else if (i >= 4 && i <= 7 && j >= 8 && j <= 11)
                    SetConsoleTextAttribute(hConsole, 7);
                else if (i >= 4 && i <= 7 && j >= 12 && j <= 15)
                    SetConsoleTextAttribute(hConsole, 6);
                else if (i >= 8 && i <= 11 && j >= 0 && j <= 3)
                    SetConsoleTextAttribute(hConsole, 6);
                else if (i >= 8 && i <= 11 && j >= 4 && j <= 7)
                    SetConsoleTextAttribute(hConsole, 7);
                else if (i >= 8 && i <= 11 && j >= 8 && j <= 11)
                    SetConsoleTextAttribute(hConsole, 6);
                else if (i >= 8 && i <= 11 && j >= 12 && j <= 15)
                    SetConsoleTextAttribute(hConsole, 7);
                else if (i >= 12 && i <= 15 && j >= 0 && j <= 3)
                    SetConsoleTextAttribute(hConsole, 7);
                else if (i >= 12 && i <= 15 && j >= 4 && j <= 7)
                    SetConsoleTextAttribute(hConsole, 6);
                else if (i >= 12 && i <= 15 && j >= 8 && j <= 11)
                    SetConsoleTextAttribute(hConsole, 7);
                else if (i >= 12 && i <= 15 && j >= 12 && j <= 15)
                    SetConsoleTextAttribute(hConsole, 6);
            }
            if(getCell(getSize()*i+j)==16) cout<<"G ";
            else if(getCell(getSize()*i+j)) cout<<uppercase<<hex<<getCell(getSize()*i+j)<<" ";
            else
                cout<<"* ";
            SetConsoleTextAttribute(hConsole, 7);
        }
        cout<<"\n";
    }
    cout<<"\n";
}
void sudoku16x16::getFile(int level)
{
    setSize(16);
    setLevel(level);
    string fname = "16x16";
    switch (level) {
        case 1: {
            fname += "easy";
        }
            break;
        case 2: {
            fname += "medium";
        }
            break;
        case 3: {
            fname += "hard";
        }
            break;
    }
    fname+=".txt";
    tab.resize(getSize() * getSize());
    solved.resize(getSize() * getSize());
    vis.resize(getSize() * getSize());
    ifstream input(fname);
    if (input) {
        for (int i = 0; i < getSize() * getSize(); i++) {
            input >> tab[i];
            if (tab[i]) {
                prefilled.push_back(true);
                fixed.push_back(true);
            }
            else {
                prefilled.push_back(false);
                fixed.push_back(false);
            }
            for (int j = 1; j <= getSize(); j++)
                vis[i].push_back(false);
            vis[i].push_back(false);
        }
        input.close();
    }
}
string player::getName() { return name; }
double player::getTime() { return time; }
int player::getHint_count() { return hint_count; }
void player::setName(string name) { this->name=name; }
void player::stopTime() {}
void player::addHint() { hint_count++; }
void game::main_menu()
{
    char a;
    system("cls");
    cout << "GRA SUDOKU\n\n1. NOWA GRA\n2. NAJLEPSI GRACZE\n3. POMOC\n\n";
    cin >> a;
    switch (a) {
        case '1':{
            new_game();
        }
            break;
        case '2': {
            leaderboard1();
        }
            break;
        case '3': {
            help();
        }
            break;
        default: {
            system("cls");
            cout<<"NIEZNANE POLECENIE\nWPISZ CYFRE ODPOWIADAJACA WYBRANEJ OPCJI I NACISNIJ ENTER\n";
            Sleep(3000);
            main_menu();
        }
            break;
    }
}
void game::new_game()
{
    char a;
    system("cls");
    cout<<"NOWA GRA\n\nWYBIERZ TRYB GRY:\n1. SUDOKU 4X4\n2. SUDOKU 9X9\n3. SUDOKU 16X16\n\n4. POWROT\n\n";
    cin>>a;
    switch(a)
    {
        case '1':
        {
            choose_level(4);
        }
            break;
        case '2':
        {
            choose_level(9);
        }
            break;
        case '3':
        {
            choose_level(16);
        }
            break;
        case '4':
        {
            main_menu();
        }
            break;
        default:
        {
            system("cls");
            cout<<"NIEZNANE POLECENIE\nWPISZ CYFRE ODPOWIADAJACA WYBRANEJ OPCJI I NACISNIJ ENTER\n";
            Sleep(3000);
            new_game();
        }
            break;
    }
}
void game::choose_level(int size)
{
    system("cls");
    cout<<"SUDOKU "<<size<<"x"<<size<<"\n\nWYBIERZ POZIOM TRUDNOSCI:\n1. LATWY\n2. SREDNI\n3. TRUDNY\nLUB\n4. ZAIMPORTUJ WLASNA PLANSZE\n\n5. POWROT\n\n";
    char b;
    cin>>b;
    int a=b-'0';
    switch(b) {
        case '1': {
            play(start(size,a),newPlayer());
        }
            break;
        case '2': {
            play(start(size,a),newPlayer());
        }
            break;
        case '3': {
            play(start(size,a),newPlayer());
        }
            break;
        case '4': {
            play(import(size),newPlayer());
        }
            break;
        case '5': {
            new_game();
        }
            break;
        default:
        {
            system("cls");
            cout<<"NIEZNANE POLECENIE\nWPISZ CYFRE ODPOWIADAJACA WYBRANEJ OPCJI I NACISNIJ ENTER\n";
            Sleep(3000);
            choose_level(size);
        }
            break;
    }
}
player *game::newPlayer()
{
    system("cls");
    player *ptr = new player;
    cout<<"PODAJ NAZWE GRACZA:\n";
    string player_name;
    cin>>player_name;
    while(player_name.size()>15)
    {
        system("cls");
        cout<<"NAZWA GRACZA NIE MOZE PRZEKRACZAC 15 ZNAKOW\nPODAJ NAZWE GRACZA:\n";
        cin>>player_name;
    }
    ptr->setName(player_name);
    return ptr;
}
sudoku *game::start(int size, int level)
{
    system("cls");
    sudoku *ptr;
    switch(size) {
        case 4: {
            ptr = new sudoku4x4;
            ptr->getFile(level);
        }
            break;
        case 9: {
            ptr = new sudoku9x9;
            ptr->getFile(level);
        }
            break;
        case 16: {
            ptr = new sudoku16x16;
            ptr->getFile(level);
        }
            break;
    }
    return ptr;

}
sudoku *game::import(int size)
{
    system("cls");
    string name;
    cout<<"PODAJ NAZWE PLIKU TXT, W KTORYM MIESCI SIE TWOJA PLANSZA\n";
    cin>>name;
    sudoku *ptr;
    switch(size) {
        case 4: {
            ptr = new sudoku4x4;
        }
            break;
        case 9: {
            ptr = new sudoku9x9;
        }
            break;
        case 16: {
            ptr = new sudoku16x16;
        }
            break;
    }
    ifstream input(name);
    ptr->setSize(size);
    ptr->setLevel(4);
    if(input)
    {
        for(int i=0; i<ptr->getSize()*ptr->getSize(); i++)
        {
            int a;
            input>>a;
            ptr->importCell(i, a, size);
        }
        input.close();
    }
    else
    {
        system("cls");
        cout<<"NIE ZNALEZIONO PLIKU O PODANEJ NAZWIE.\n\n";
        Sleep(3000);
        choose_level(size);
    }
    return ptr;
}
void game::play(sudoku *ptr, player *p1)
{
    string level_name;
    switch(ptr->getLevel())
    {
        case 1:
        {
            level_name="LATWY";
        }
            break;
        case 2:
        {
            level_name="SREDNI";
        }
            break;
        case 3:
        {
            level_name="TRUDNY";
        }
            break;
    }
    auto begin = std::chrono::high_resolution_clock::now();
    while(!ptr -> done())
    {
        system("cls");
        if(ptr->getLevel()>3) cout<<"SUDOKU "<<ptr->getSize()<<"x"<<ptr->getSize()<<" - NIESTANDARDOWY\n\n";
        else cout<<"SUDOKU "<<ptr->getSize()<<"x"<<ptr->getSize()<<" - "<<level_name<<"\n\n";
        ptr -> show();
        ptr -> reset_hint();
        char a;
        char w,k,n;
        int x,y,z;
        cout<<"1. UZUPELNIJ KOMORKE\n2. COFNIJ\n3. PODPOWIEDZ\n4. POMOC\n5. WYJDZ Z GRY\n\n";
        cin>>a;
        switch(a)
        {
            case '1':
            {
                cout<<"PODAJ WSPOLRZEDNE\nW: ";
                cin>>uppercase>>w;
                cout<<"K: ";
                cin>>uppercase>>k;
                if(w>='1' && w<='9')
                    x=w-'0';
                else if(w>='A' && w<='G')
                    x=w-'7';
                if(k>='1' && k<='9')
                    y=k-'0';
                else if(k>='A' && k<='G')
                    y=k-'7';
                if(x>ptr->getSize() || y>ptr->getSize() || x<0 || y<0)
                {
                    system("cls");
                    if(ptr->getLevel()>3) cout<<"SUDOKU "<<ptr->getSize()<<"x"<<ptr->getSize()<<" - NIESTANDARDOWY\n\n";
                    else cout<<"SUDOKU "<<ptr->getSize()<<"x"<<ptr->getSize()<<" - "<<level_name<<"\n\n";
                    ptr->show();
                    cout<<"NIEWLASCIWE WSPOLRZEDNE";
                    Sleep(3000);
                    play(ptr,p1);
                }
                if(ptr->wasPrefilled(ptr->getSize()*(x-1)+y-1))
                {
                    system("cls");
                    if(ptr->getLevel()>3) cout<<"SUDOKU "<<ptr->getSize()<<"x"<<ptr->getSize()<<" - NIESTANDARDOWY\n\n";
                    else cout<<"SUDOKU "<<ptr->getSize()<<"x"<<ptr->getSize()<<" - "<<level_name<<"\n\n";
                    ptr->show();
                    cout<<"NIE MOZESZ ZMIENIC ZAWARTOSCI TEJ KOMORKI";
                    Sleep(3000);
                    continue;
                }
                cout<<"PODAJ CYFRE: ";
                cin>>n;
                if(n>='1' && n<='9')
                    z=n-'0';
                else if(n>='A' && n<='G')
                    z=n-'7';
                else
                    z=-1;
                if(z<1 || z>ptr->getSize())
                {
                    system("cls");
                    if(ptr->getLevel()>3) cout<<"SUDOKU "<<ptr->getSize()<<"x"<<ptr->getSize()<<" - NIESTANDARDOWY\n\n";
                    else cout<<"SUDOKU "<<ptr->getSize()<<"x"<<ptr->getSize()<<" - "<<level_name<<"\n\n";
                    ptr->show();
                    cout<<"NIEWLASCIWA CYFRA";
                    Sleep(3000);
                    continue;
                }
                ptr -> insert(x-1,y-1,z);
            }
                break;
            case '2':
            {
                ptr -> remove();
            }
                break;
            case '3':
            {
                if(p1->getHint_count()==0)
                {
                    system("cls");
                    if(ptr->getLevel()>3) cout<<"SUDOKU "<<ptr->getSize()<<"x"<<ptr->getSize()<<" - NIESTANDARDOWY\n\n";
                    else cout<<"SUDOKU "<<ptr->getSize()<<"x"<<ptr->getSize()<<" - "<<level_name<<"\n\n";
                    ptr->show();
                    cout<<"UWAGA, JESLI SKORZYSTASZ ZE WSKAZOWKI TWOJ CZAS NIE POJAWI SIE W TABELI NAJLEPSZYCH GRACZY\nKONTYNUOWAC?\n1. TAK\n2. NIE\n";
                    cin>>a;
                    if(a==1)
                    {
                        ptr -> hint();
                        p1 -> addHint();
                    }
                    if(a==2)
                    {
                        continue;
                    }
                }
                ptr -> hint();
                p1 -> addHint();
            }
                break;
            case '4':
            {
                rules(ptr,p1);
            }
                break;
            case '5':
            {
                main_menu();
            }
                break;
            default:
            {
                system("cls");
                cout<<"NIEZNANE POLECENIE\nWPISZ CYFRE ODPOWIADAJACA WYBRANEJ OPCJI I NACISNIJ ENTER\n";
                Sleep(3000);
                play(ptr,p1);
            }
                break;
        }
    }
    system("cls");
    if(ptr->getLevel()>3) cout<<"SUDOKU "<<ptr->getSize()<<"x"<<ptr->getSize()<<" - NIESTANDARDOWY\n\n";
    else cout<<"SUDOKU "<<ptr->getSize()<<"x"<<ptr->getSize()<<" - "<<level_name<<"\n\n";
    ptr -> show();
    if(ptr->win())
    {
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - begin;
        cout<<"GRATULACJE "<<p1->getName()<<"\n\nTWOJ CZAS: "<<setprecision(3)<<elapsed.count()<<"s\nLICZBA UZYTYCH PODPOWIEDZI: "<<p1->getHint_count()<<"\n";
        if(p1->getHint_count()==0)
        addToBoard(ptr->getSize(),ptr->getLevel(),p1->getName(),elapsed.count());
    }
    else
    {
        cout<<"SUDOKU UZUPELNIONE NIEPOPRAWNIE. SPROBUJ PONOWNIE\n\n";
        Sleep(3000);
        ptr->remove();
        play(ptr,p1);
    }
    system ("pause");
    main_menu();
}
void game::leaderboard1()
{
    char a;
    system("cls");
    cout<<"TABLICA WYNIKOW\n\nWYBIERZ TRYB GRY\n1. SUDOKU 4X4\n2. SUDOKU 9X9\n3. SUDOKU 16X16\n\n4. POWROT\n\n";
    cin>>a;
    switch(a) {
        case '1': {
            leaderboard2(4);
        }
            break;
        case '2': {
            leaderboard2(9);
        }
            break;
        case '3': {
            leaderboard2(16);
        }
            break;
        case '4': {
            main_menu();
        }
            break;
        default:
        {
            system("cls");
            cout<<"NIEZNANE POLECENIE\nWPISZ CYFRE ODPOWIADAJACA WYBRANEJ OPCJI I NACISNIJ ENTER\n";
            Sleep(3000);
            leaderboard1();
        }
            break;
    }
}
void game::leaderboard2(int size)
{
    system("cls");
    cout<<"TABLICA WYNIKOW\nSUDOKU "<<size<<"x"<<size<<"\n\nWYBIERZ POZIOM TRUDNOSCI\n1. LATWY\n2. SREDNI\n3. TRUDNY\n\n4. POWROT\n\n";
    char b;
    cin>>b;
    int a=b-'0';
    switch(b) {
        case '1': {
        showBoard(size,a);
            cout<<"1. POWROT\n\n";
            cin>>a;
            if(a==1)
            {
                leaderboard2(size);
            }
        }
            break;
        case '2': {
            showBoard(size,a);
            Sleep(3000);
            leaderboard2(size);
        }
            break;
        case '3': {
            showBoard(size,a);
            Sleep(3000);
            leaderboard2(size);
        }
            break;
        case '4': {
                leaderboard1();
        }
            break;
        default:
        {
            system("cls");
            cout<<"NIEZNANE POLECENIE\nWPISZ CYFRE ODPOWIADAJACA WYBRANEJ OPCJI I NACISNIJ ENTER\n";
            Sleep(3000);
            leaderboard2(size);
        }
            break;
    }

}
void game::addToBoard(int size, int level, string name, double time)
{
    pair <int, int> mypair=make_pair(size,level);
    board[mypair].push_back(make_pair(name, time));
    int index = board[mypair].size() - 1;
    cout<<index<<" ";
    if(board[mypair].size()>1)
    while(board[mypair][index].second<board[mypair][index-1].second && index>0)
    {
        swap(board[mypair][index],board[mypair][index-1]);
        index--;
        if(index==0) break;
    }
}

void game::showBoard(int size, int level)
{
    system("cls");
    string level_name;
    switch(level)
    {
        case 1:
        {
            level_name="LATWY";
        }
            break;
        case 2:
        {
            level_name="SREDNI";
        }
            break;
        case 3:
        {
            level_name="TRUDNY";
        }
            break;
    }

    cout<<"TABLICA WYNIKOW\nSUDOKU "<<size<<"x"<<size<<" - "<<level_name<<"\n\n";
    if(board[make_pair(size,level)].size()==0)
    {
        cout<<"BRAK NAJLEPSZYCH WYNIKOW\n";
    }
    else
        {
        cout << "LP.    NAZWA GRACZA     CZAS\n";
        for (int i = 0; i < board[make_pair(size, level)].size(); i++) {
            cout << " "<<i + 1 << "." << setw(16) << board[make_pair(size, level)][i].first
                 << "       "<<board[make_pair(size, level)][i].second << "s\n";
        }
    }
    cout<<"\n";
}
void game::help()
{
    system("cls");
    cout<<"WPISZ CYFRE ODPOWIADAJACA WYBRANEJ OPCJI I NACISNIJ ENTER\n\n1. POWROT\n\n";
    int a;
    cin>>a;
    if(a==1)
    {
        main_menu();
    }
    else
    {
        system("cls");
        cout<<"NIEZNANE POLECENIE\nWPISZ CYFRE ODPOWIADAJACA WYBRANEJ OPCJI I NACISNIJ ENTER\n";
        Sleep(3000);
        help();
    }
}
void game::rules(sudoku *ptr, player *p1)
{
    system("cls");
    cout<<"SUDOKU - ZASADY GRY:\nCELEM GRY JEST WYPELNIENIE PLANSZY CYFRAMI (LUB CYFRAMI I LITERAMI),\nTAK ABY W KAZDYM WIERSZU, KOLUMNIE I KWADRACIE, NA KTORE PODZIELONA JEST PLANSZA,\nZNALAZLA SIE DOKLADNIE JEDNA CYFRA (LUB CYFRA I LITERA) KAZDEGO RODZAJU.\n\n";
    cout<<"WPISZ CYFRE ODPOWIADAJACA WYBRANEJ OPCJI I NACISNIJ ENTER\n\n1. POWROT\n\n";
    int a;
    cin>>a;
    if(a==1)
    {
        play(ptr,p1);
    }
    else
    {
        system("cls");
        cout<<"NIEZNANE POLECENIE\nWPISZ CYFRE ODPOWIADAJACA WYBRANEJ OPCJI I NACISNIJ ENTER\n";
        Sleep(3000);
        rules(ptr,p1);
    }
}