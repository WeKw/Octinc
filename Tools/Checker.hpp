/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_CHECKER_HPP
#  define OCTINC_CHECKER_HPP
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif

namespace Octinc {
class Checker {
   private:
    std::string _prog, _infile, _outfile, _ansfile;

   public:
    static const int ACCEPT_STATE = 0;
    static const int WRONG_ANSWER = 1;
    static const int ANSWER_TOO_SHORT = 2;

    struct Result {
        clock_t _time;
        int state;
        int first_line;

        Result(clock_t t, int s, int f) : _time(t), state(s), first_line(f) {}
    };

    Checker() {}
    Checker(std::string p, std::string i, std::string o, std::string a)
        : _prog(p), _infile(i), _outfile(o), _ansfile(a) {}
    /*
    _prog is the name of program;
    _infile is the name of input file;
    _outfile is the name of output file;
    _ansfile is the name of answer file.
    */

    void build(std::string p, std::string i, std::string o, std::string a) {
        _prog = p;
        _infile = i;
        _outfile = o;
        _ansfile = a;
    }

    Result operator()(int i) {
        clock_t start = clock();
        system(std::string(_prog + ".exe" + " < " + _infile +
                           std::to_string(i) + ".in" + " > " + _outfile +
                           ".out")
                   .c_str());

        std::ifstream fin(std::string(_infile + ".out").c_str()),
            fans(std::string(_ansfile + std::to_string(i) + ".ans").c_str());

        start = clock() - start;
        std::string _inr, _ansr;
        int line = 0;

        while (std::getline(fans, _ansr)) {
            ++line;

            if (!std::getline(fin, _inr))
                return Result(start, ANSWER_TOO_SHORT, 0);

            while (_ansr.back() == ' ') _ansr.pop_back();

            while (_inr.back() == ' ') _inr.pop_back();

            if (_inr != _ansr) return Result(start, WRONG_ANSWER, line);
        }

        return Result(start, ACCEPT_STATE, -1);
    }
};
}  // namespace Octinc

#endif
