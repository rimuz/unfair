#include "save.hpp"
#include "engine/scene.hpp"

#include <fstream>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <Shlobj.h>
#endif

namespace ue{
    namespace save{
        std::vector<Score> score;
        Options options;
        std::string scores_path("./scores.bin");
        std::string options_path("./options.bin");

        #ifdef _WIN32
        TCHAR tpath[MAX_PATH];
        void init(){
            SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, tpath);
            std::basic_string<TCHAR> appdata(tpath);
            std::string appdata_str(appdata);
            appdata_str += "\\Unfair";
            CreateDirectory(appdata_str.data(), NULL);

            scores_path = appdata_str + "\\scores.bin";
            options_path = appdata_str + "\\options.bin";
        }
        #endif

        void load(){
            std::ifstream scores_file(scores_path, std::ios::in | std::ios::binary);
            if(scores_file){
                scores_file.seekg(0, std::ios_base::end);
                const std::size_t sz = scores_file.tellg() / sizeof(Score);
                scores_file.seekg(0);
                score.reserve(sz);
                Score sc;
                for(std::size_t i = 0; i < sz; i++){
                    scores_file.read(reinterpret_cast<char*>(&sc), sizeof(sc));
                    score.emplace_back(sc);
                }
                scores_file.close();
            }

            std::ifstream options_file(options_path, std::ios::in | std::ios::binary);
            if(options_file){
                options_file.read(reinterpret_cast<char*>(&options), sizeof(options));
                options_file.close();
            }

        }


        void save(){
            save::options.winPos = SCENE.window.getPosition();
            std::ofstream scores_file (scores_path, std::ios::out | std::ios::binary);
            if(scores_file){
                Score sc;
                for(std::size_t i = 0; i < score.size(); i++){
                    sc = score[i];
                    scores_file.write(reinterpret_cast<const char*>(&sc), sizeof(sc));
                }
                scores_file.close();
            }

            std::ofstream options_file (options_path, std::ios::out | std::ios::binary);
            if(options_file){
                options_file.write(reinterpret_cast<const char*>(&options), sizeof(options));
                options_file.close();
            }
        }
    }
}
