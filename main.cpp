#include "lib/thinger/thinger.h"
#include <string.h>
#include <cstring>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <fstream>

using namespace std;

#undef FALSE
#define FALSE 0
#undef TRUE
#define TRUE 1


string exec(string cmdStr);
bool occupied = false;
string initiator;
string ZAPIER_URL;


int PostJSON(string message, string channelID, string SzapierURL)
{
  int retcode = FALSE;
  cJSON *root = NULL, *out = NULL;
  char *json = NULL;
  CURL *curl = NULL;
  CURLcode res = CURLE_FAILED_INIT;
  char errbuf[CURL_ERROR_SIZE] = { 0, };
  struct curl_slist *headers = NULL;
  char agent[1024] = { 0, };
  const char* CzapierURL = SzapierURL.c_str();

    char channel[channelID.size() + 1];
    strcpy(channel, channelID.c_str());
    char text[message.size() + 1];
    strcpy(text, message.c_str());

  root = cJSON_CreateObject();
  if(!root) {
    cout << "Error: cJSON_CreateObject failed";
    goto cleanup;
  }

  cJSON_AddItemToArray(root, out = cJSON_CreateObject());
  cJSON_AddStringToObject(out, "text", text);
  cJSON_AddStringToObject(out, "userName", "Meetup");
  cJSON_AddStringToObject(out, "channelID", channel);

  json = cJSON_PrintUnformatted(root);
  if(!json) {
    cout << "Error: cJSON_PrintUnformatted failed";
    goto cleanup;
  }

  curl = curl_easy_init();
  if(!curl) {
    cout << "Error: curl_easy_init failed.";
    goto cleanup;
  }
  //curl_easy_setopt(curl, CURLOPT_CAINFO, "curl-ca-bundle.crt");
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);

  snprintf(agent, sizeof agent, "libcurl/%s", curl_version_info(CURLVERSION_NOW)->version);
  agent[sizeof agent - 1] = 0;
  curl_easy_setopt(curl, CURLOPT_USERAGENT, agent);

  headers = curl_slist_append(headers, "Expect:");
  headers = curl_slist_append(headers, "Content-Type: application/json");
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);

  /* This is a test server, it fakes a reply as if the json object were
     created */
  curl_easy_setopt(curl, CURLOPT_URL, CzapierURL);

  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

  res = curl_easy_perform(curl);
  if(res != CURLE_OK) {
    size_t len = strlen(errbuf);
    cout << "errore boh";
    //fprintf(stderr, "\nlibcurl: (%d) ", res);
    if(len){
    //fprintf(stderr, "%s%s", errbuf, ((errbuf[len - 1] != '\n') ? "\n" : ""));
    cout << "errore boh";
    }

    //fprintf(stderr, "%s\n\n", curl_easy_strerror(res));
    cout << "errore boh";
    goto cleanup;
  }

  retcode = TRUE;

cleanup:
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
  cJSON_Delete(root);
  free(json);
  return retcode;
}





int main(int argc, char *argv[]){
    string line[5];
    ifstream myfile ("settings.txt");
    int i = 0;
    if(myfile.is_open()){

        while(getline(myfile,line[i])){
                cout << line[i] << '\n';
                i++;
            }
cout << "ciao" <<'\n';
        myfile.close();
    }
    else cout << "Unable to open file";

    for(int i = 0; i < 4; i++){
        int pos = line[i].find("=");
        line[i] = line[i].substr(pos + 2);
    }
    const char* USER_ID = line[0].c_str();
    const char* DEVICE_ID = line[1].c_str();
    const char* DEVICE_CREDENTIAL = line[2].c_str();
     ZAPIER_URL = line[3];

    thinger_device thing(USER_ID, DEVICE_ID, DEVICE_CREDENTIAL);

    // define thing resources here. i.e, this is a sum example
    thing["new-message"] = [](pson& in, pson& out){
        string message = "";
        bool help;
        string url, result;
        string text = in["text"];
        string userName = in["userName"];
        string channelName = in["channelName"];
        string channelID = in["channelID"];
        system("echo ciao");
        transform(text.begin(), text.end(), text.begin(), ::tolower);
        if(text.find("@meetup") != std::string::npos){
            help = true;
            system("echo trigger@meetup");
            /*
            result = exec("./test-begin.sh");
            cout << result;
            system("echo trigger@meetup fine");
            */
            if(
                (text.find("start") != std::string::npos) ||
                (text.find("inizia") != std::string::npos) ||
                (text.find("begin") != std::string::npos)){
                    help = false;
                    system("echo trigger@start");
                    cout << occupied;
                    if(occupied == false){
                        occupied = true;
                        initiator = userName;
                        system("echo trigger@start");
                        result = exec("./begin-call.sh");
                        //result= exec("./test-begin.sh");
                        message = "Ciao @";
                        message += userName;
                        message += "  \n You can join the meet with link: \n";
                        message += result;
                        PostJSON(message, channelID, ZAPIER_URL);
                    }else{
                        message = "Ciao @";
                        message += userName;
                        message += "  \n There is another meet running at the moment, started by ";
                        message += initiator;
                        message += "\n you can stop it with @meetup kill call";
                        PostJSON(message, channelID, ZAPIER_URL);
                    }
                system("echo trigger@mstart fine");
            }
            if(
                (text.find("stop") != std::string::npos) ||
                (text.find("termina") != std::string::npos) ||
                (text.find("kill") != std::string::npos) ||
                (text.find("chiudi") != std::string::npos)){
                help = false;
                system("echo trigger@stop");
                if(occupied == true){
                    system("./kill-call.sh");
                    message = "Ciao @";
                    message += userName;
                    message += "  \n I killed the meet that was running \n you can start a new one with @meetup begin call";
                    PostJSON(message, channelID, ZAPIER_URL);
                    occupied = false;
                }else{
                    message = "Ciao @";
                    message += userName;
                    message += "  \n There are no meets running at the moment \n you can start one with @meetup begin call";
                    PostJSON(message, channelID, ZAPIER_URL);
                    }
                system("echo trigger@stop fine");
            }
            if(text.find("join") != std::string::npos){
                help = false;
                system("echo trigger@join");
                if(occupied == false){
                    cout << text << '\n';
                    size_t found = text.find("meet.google.com");
                    if (found!=std::string::npos){
                        cout << "first 'meet.google.com' found at: " << found << '\n';
                        string joinURL = text.substr (found, 28);
                        cout << joinURL << '\n';

                        string commandJoin = "./join-call.sh ";
                        commandJoin += joinURL;
                        const char *command = commandJoin.c_str();
                        system(command );
                    }
                    else{
                        cout << "meet not found" << '\n';
                        message = "Ciao @";
                        message += userName;
                        message += "  \n I couldn't find a vallid google meet URL, \n try again";
                        PostJSON(message, channelID, ZAPIER_URL);
                    }
                }else{
                    message = "Ciao @";
                    message += userName;
                    message += "  \n There is another meet running at the moment, started by ";
                    message += initiator;
                    message += "\n you can stop it with @meetup kill call";
                    PostJSON(message, channelID, ZAPIER_URL);
                }
                system("echo trigger@join fine");
            }
            if(
                (text.find("status") != std::string::npos) ||
                (text.find("stato") != std::string::npos)){
                help = false;
                system("echo trigger@status");
                if(occupied == true){
                    message = "Ciao @";
                    message += userName;
                    message += "  \n There is a meet started by";
                    message += initiator;
                    PostJSON(message, channelID, ZAPIER_URL);
                }else{
                    message = "Ciao @";
                    message += userName;
                    message += "  \n There are no meets running at the moment \n you can start one with @meetup begin call";
                    PostJSON(message, channelID, ZAPIER_URL);
                    }
                system("echo trigger@status fine");
            }
            if((text.find("help") != std::string::npos) || help){
                help = false;
                system("echo trigger@help");
            }
        }

        out["response"] = "ciaone";
    };
    thing["status"] = [](pson& in, pson& out){
        out["status"] = "ciao";
    };
    thing.start();
    return 0;
}

string exec(string cmdStr){
    const char *cmd = cmdStr.c_str();
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}





