#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <limits>
using namespace std;

struct Notification {
    int id;
    string category,title,message,priority,time;
    bool read;
};

vector<Notification> n;
int nextID=1001;
const string FILE_NAME="notifications.dat";

string now(){
    time_t t=time(0); tm *x=localtime(&t); char s[25];
    strftime(s,25,"%d-%m-%Y %H:%M:%S",x);
    return s;
}

void line(char c='=',int x=70){cout<<string(x,c)<<'\n';}

void clearInput(){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
}

void save(){
    ofstream f(FILE_NAME);
    for(auto &a:n)
        f<<a.id<<'|'<<a.category<<'|'<<a.title<<'|'<<a.message
         <<'|'<<a.priority<<'|'<<a.time<<'|'<<a.read<<'\n';
}
void load(){
    ifstream f(FILE_NAME);
    string s;
    while(getline(f,s)){
        stringstream x(s),id,read;
        Notification a;
        string I,R;
        getline(x,I,'|'); a.id=stoi(I);
        getline(x,a.category,'|');
        getline(x,a.title,'|');
        getline(x,a.message,'|');
        getline(x,a.priority,'|');
        getline(x,a.time,'|');
        getline(x,R,'|');
        a.read=(R=="1");
        n.push_back(a);
        nextID=max(nextID,a.id+1);
    }
}

void create(string c,string t,string m,string p){
    Notification a{nextID++,c,t,m,p,now(),false};
    n.push_back(a);

    cout<<"\n"; line();
    cout<<" NOTIFICATION CREATED\n"; line();
    cout<<"Notification ID : "<<a.id
        <<"\nCategory : "<<a.category
        <<"\nTitle : "<<a.title
        <<"\nPriority : "<<a.priority
        <<"\nStatus : UNREAD"
        <<"\nTime : "<<a.time<<"\n";
    line();
    save();
}

void show(const Notification &a){
    cout<<"\n"; line('-');
    cout<<"ID : "<<a.id
        <<"\nCategory : "<<a.category
        <<"\nTitle : "<<a.title
        <<"\nMessage : "<<a.message
        <<"\nPriority : "<<a.priority
        <<"\nStatus : "<<(a.read?"READ":"UNREAD")
        <<"\nTime : "<<a.time<<"\n";
    line('-');
}
/* Monitoring */

void monitor(string type){
    double v;
    cout<<"\n"; line();
    cout<<" "<<type<<" MONITOR\n"; line();

    cout<<"Enter "<<(type=="BATTERY"?
        "battery percentage (0-100): ":"device temperature (C): ");
    cin>>v;

    if(type=="BATTERY"){
        if(v<0||v>100){cout<<"\nInvalid battery value.\n";return;}

        int i=v<=5?0:v<=15?1:v<=30?2:3;
        string p[]={"CRITICAL","HIGH","MEDIUM","LOW"};
        string t[]={"Critical Battery Level","Low Battery",
                    "Battery Warning","Battery Status"};
        string m[]={
            "Battery is critically low. Connect charger immediately.",
            "Battery level is very low. Charging is recommended.",
            "Battery level is getting low.",
            "Battery level is currently normal."
        };
        create("BATTERY",t[i],m[i],p[i]);
    }
    else{
        int i=v>=45?0:v>=40?1:v>=35?2:3;
        string p[]={"CRITICAL","HIGH","MEDIUM","LOW"};
        string t[]={"Critical Overheating","High Temperature",
                    "Temperature Warning","Temperature Normal"};
        string m[]={
            "Device temperature is extremely high.",
            "Device temperature is dangerously high.",
            "Device temperature is above normal.",
            "Device temperature is within normal range."
        };
        create("TEMPERATURE",t[i],m[i],p[i]);
    }
}
/* Network */

void network(){
    int c;
    cout<<"\n";line();
    cout<<" NETWORK MONITOR\n";line();
    cout<<"1. Connected\n2. Weak Connection\n3. Disconnected\n";
    cout<<"\nSelect status: ";cin>>c;

    if(c<1||c>3){cout<<"\nInvalid network status.\n";return;}

    string t[]={"","Network Connected","Weak Network",
                "Network Disconnected"};
    string m[]={"","Network connection is active.",
                "Network connection appears to be unstable.",
                "No network connection is currently available."};
    string p[]={"","LOW","MEDIUM","HIGH"};

    create("NETWORK",t[c],m[c],p[c]);
}

/* Reminder */

void reminder(){
    clearInput();
    string t,m;

    cout<<"\n";line();
    cout<<" REMINDER SYSTEM\n";line();

    cout<<"Enter reminder title: ";getline(cin,t);
    cout<<"Enter reminder message: ";getline(cin,m);

    if(t.empty()||m.empty()){
        cout<<"\nReminder information cannot be empty.\n";return;
    }

    create("REMINDER",t,m,"MEDIUM");
}
/* Custom Notification */

void custom(){
    clearInput();
    string c,t,m;int p;

    cout<<"\n";line();
    cout<<" CREATE CUSTOM NOTIFICATION\n";line();

    cout<<"Category : ";getline(cin,c);
    cout<<"Title : ";getline(cin,t);
    cout<<"Message : ";getline(cin,m);

    cout<<"\nPriority Options:\n1. LOW\n2. MEDIUM\n3. HIGH\n4. CRITICAL\n";
    cout<<"\nSelect priority: ";cin>>p;

    if(p<1||p>4){cout<<"\nInvalid priority.\n";return;}

    string P[]={"","LOW","MEDIUM","HIGH","CRITICAL"};
    create(c,t,m,P[p]);
}

/* View */

void inbox(bool unread=false){
    cout<<"\n";line();
    cout<<(unread?
        " UNREAD NOTIFICATIONS\n":
        " NOTIFICATION INBOX\n");
    line();

    bool found=false;
    for(auto i=n.rbegin();i!=n.rend();i++)
        if(!unread||!i->read){show(*i);found=true;}

    if(!found)cout<<"\nNo notifications available.\n";
}

/* Search */

void search(){
    clearInput();
    string k;
    cout<<"\nEnter keyword to search: ";getline(cin,k);

    bool found=false;

    for(auto &a:n){
        string s=a.category+" "+a.title+" "+a.message+" "+a.priority;
        if(s.find(k)!=string::npos){show(a);found=true;}
    }

    if(!found)cout<<"\nNo matching notifications found.\n";
}
/* Priority */

void filter(){
    int c;
    cout<<"\n";line();
    cout<<" PRIORITY FILTER\n";line();
    cout<<"1. CRITICAL\n2. HIGH\n3. MEDIUM\n4. LOW\n";
    cout<<"\nSelect priority: ";cin>>c;

    if(c<1||c>4){cout<<"\nInvalid option.\n";return;}

    string p[]={"","CRITICAL","HIGH","MEDIUM","LOW"};
    bool found=false;

    for(auto &a:n)
        if(a.priority==p[c]){show(a);found=true;}

    if(!found)cout<<"\nNo notifications with this priority.\n";
}

/* Management */

void markRead(){
    int id;
    cout<<"\nEnter Notification ID: ";cin>>id;

    for(auto &a:n)
        if(a.id==id){
            a.read=true;save();
            cout<<"\nNotification marked as READ successfully.\n";
            return;
        }

    cout<<"\nNotification ID not found.\n";
}

void removeNote(){
    int id;
    cout<<"\nEnter Notification ID to delete: ";cin>>id;

    auto old=n.size();

    n.erase(remove_if(n.begin(),n.end(),
        [id](const Notification&a){return a.id==id;}),n.end());

    if(n.size()<old){
        save();
        cout<<"\nNotification deleted successfully.\n";
    }else cout<<"\nNotification ID not found.\n";
}

void clearAll(){
    char c;
    cout<<"\nWARNING: This will delete ALL notifications.\n";
    cout<<"Continue? (Y/N): ";cin>>c;

    if(c=='Y'||c=='y'){
        n.clear();save();
        cout<<"\nAll notification data has been cleared.\n";
    }else cout<<"\nOperation cancelled.\n";
}
/* Dashboard */

void dashboard(){
    int unread=0,critical=0;

    for(auto &a:n){
        unread+=!a.read;
        critical+=a.priority=="CRITICAL";
    }

    cout<<"\n";line();
    cout<<" SMART NOTIFICATION SYSTEM\n";
    cout<<" DASHBOARD\n";line();

    cout<<"\nSYSTEM STATUS : ONLINE"
        <<"\nNOTIFICATION ENGINE : ACTIVE"
        <<"\nDATABASE STATUS : READY"
        <<"\nCurrent Time : "<<now()
        <<"\nTotal Notifications : "<<n.size()
        <<"\nUnread : "<<unread
        <<"\nCritical Alerts : "<<critical
        <<"\nNext Notification ID: "<<nextID<<"\n";
    line();
}

/* Statistics */

void statistics(){
    int r=0,u=0,c=0,h=0,m=0,l=0;

    for(auto &a:n){
        a.read?r++:u++;
        if(a.priority=="CRITICAL")c++;
        else if(a.priority=="HIGH")h++;
        else if(a.priority=="MEDIUM")m++;
        else l++;
    }

    cout<<"\n";line();
    cout<<" NOTIFICATION ANALYTICS\n";line();

    cout<<"\nTotal Notifications : "<<n.size()
        <<"\nUnread : "<<u
        <<"\nRead : "<<r
        <<"\n\nPriority Distribution\n"
        <<"CRITICAL : "<<c
        <<"\nHIGH : "<<h
        <<"\nMEDIUM : "<<m
        <<"\nLOW : "<<l<<"\n";

    line();
}

/* Menu */

void menu(){
    cout<<"\n\n";line();
    cout<<" SMART NOTIFICATION SYSTEM\n";line();

    cout<<"\n 1. System Dashboard"
        <<"\n 2. Notification Inbox"
        <<"\n 3. View Unread Notifications"
        <<"\n 4. Battery Monitoring"
        <<"\n 5. Temperature Monitoring"
        <<"\n 6. Network Monitoring"
        <<"\n 7. Create Reminder"
        <<"\n 8. Create Custom Notification"
        <<"\n 9. Mark Notification as Read"
        <<"\n10. Search Notifications"
        <<"\n11. Filter by Priority"
        <<"\n12. Notification Analytics"
        <<"\n13. Delete Notification"
        <<"\n14. Clear All Notifications"
        <<"\n15. Exit\n";

    line('-');
}

/* Main */
int main(){
    load();

    cout<<"\n";line();
    cout<<" SMART NOTIFICATION MANAGEMENT SYSTEM\n";line();

    cout<<"\nInitializing System...\n"
        <<"Loading Notification Database...\n"
        <<"Starting Notification Engine...\n"
        <<"System Status: ONLINE\n";

    while(true){
        menu();

        int c;
        cout<<"\nEnter your choice: ";
        cin>>c;

        if(cin.fail()){
            clearInput();
            cout<<"\nInvalid input. Please enter a number.\n";
            continue;
        }

        switch(c){
            case 1:dashboard();break;
            case 2:inbox();break;
            case 3:inbox(true);break;
            case 4:monitor("BATTERY");break;
            case 5:monitor("TEMPERATURE");break;
            case 6:network();break;
            case 7:reminder();break;
            case 8:custom();break;
            case 9:markRead();break;
            case 10:search();break;
            case 11:filter();break;
            case 12:statistics();break;
            case 13:removeNote();break;
            case 14:clearAll();break;

            case 15:
                save();
                cout<<"\n";line();
                cout<<" THANK YOU FOR USING THE SYSTEM\n";
                cout<<" SYSTEM SHUTDOWN\n";
                line();
                return 0;

            default:
                cout<<"\nInvalid choice. Please select 1-15.\n";
        }
    }
}

