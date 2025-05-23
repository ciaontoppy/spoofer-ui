#pragma once
#include "auth.hpp"
#include "skStr.h"

using namespace KeyAuth;

inline std::string name = skCrypt("tests").decrypt(); // Application Name
inline std::string ownerid = skCrypt("TzuQ9RxntH").decrypt(); // Owner ID
inline std::string secret = skCrypt("d2d351718a7bd6623245075244f7d0711488cd3f12b57079b362128da2ef096b").decrypt(); // Application Secret
inline std::string version = skCrypt("1.0").decrypt(); // Application Version
inline std::string url = skCrypt("https://keyauth.win/api/1.2/").decrypt(); // change if you're self-hosting
inline std::string path = skCrypt("").decrypt(); // (OPTIONAL) see tutorial here https://www.youtube.com/watch?v=I9rxt821gMk&t=1s

inline api KeyAuthApp(name, ownerid, secret, version, url, path);

