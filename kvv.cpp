#include <curl/curl.h>
#include <string>
#include <sstream>
#include <iostream>
#include <json/json.h>
#include <vector>
#include <ncurses.h>
#include <unistd.h>

using namespace std;

struct Departure {
	string route, destination, time;
};

const string BASE_URL = "https://live.kvv.de/webapp";
const string API_KEY  = "377d840e54b59adbe53608ba1aad70e8";

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
	data->append((char*) ptr, size * nmemb);
	return size * nmemb;
}

void getDepartures(vector<Departure> &departures, const string &stopId, string &stopName, string &timestamp) {

	stringstream ss;
	ss << BASE_URL << "/departures/bystop/" << stopId << "?key=" << API_KEY;
	ss << "&maxInfos=10";

	auto curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, ss.str().c_str());
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.42.0");
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
	curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

	std::string response;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	Json::CharReaderBuilder builder;
	Json::CharReader *reader = builder.newCharReader();
	Json::Value root;
	string errors;
	reader->parse(response.c_str(), response.c_str() + response.size(), &root, &errors);

	stopName  = root["stopName"].asString();
	timestamp = root["timestamp"].asString();
	auto departures_json = root["departures"];

	for (const auto &departure_json : departures_json) {
		Departure departure;

		departure.route       = departure_json["route"].asString();
		departure.destination = departure_json["destination"].asString();
		departure.time        = departure_json["time"].asString();

		departures.push_back(departure);
	}
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "de_DE.UTF-8");

	string stopId = "de:8212:72";
	if (argc == 2) {
		stopId = argv[1];
	}

	initscr();
	curs_set(0);

	while (1) {
		vector<Departure> departures;
		string stopName, timestamp;

		getDepartures(departures, stopId, stopName, timestamp);
		mvprintw(0, 0, "%s", stopName.c_str());

		stringstream underline;
		for (int i = 0; i < stopName.length(); i++) {underline << "-";}
		mvprintw(1, 0, "%s", underline.str().c_str());

		int i = 3;
		for (const auto &departure : departures) {
			stringstream ss;
			ss << departure.route.c_str() << "\t";
			ss << departure.destination.c_str() << "\t\t";
			ss << departure.time.c_str();

			mvprintw(i, 0, "%s", ss.str().c_str());
			i++;
		}
		refresh();
		sleep(1);
	}

	endwin();

	return 0;
}