/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
/*
Help:
Usage:
 Find the first match of the regular expression "pattern" in the supplied
string, starting at position "index".

 If found, returns REGEXP_MATCHED (1).

 Also match_start and match_len in the MatchState structure are set to the start
offset and length of the match.

 The capture in the MatchState structure has the locations and lengths of each
capture.

 If not found, returns REGEXP_NOMATCH (0).

 On a parsing error (eg. trailing % symbol) returns a negative number.

Patterns:
 The standard patterns (character classes) you can search for are:


 . --- (a dot) represents all characters.
 %a --- all letters.
 %c --- all control characters.
 %d --- all digits.
 %l --- all lowercase letters.
 %p --- all punctuation characters.
 %s --- all space characters.
 %u --- all uppercase letters.
 %w --- all alphanumeric characters.
 %x --- all hexadecimal digits.
 %z --- the character with hex representation 0x00 (null).
 %% --- a single '%' character.

 %1 --- captured pattern 1.
 %2 --- captured pattern 2 (and so on).
 %f[s]  transition from not in set 's' to in set 's'.
 %b()   balanced pair ( ... )


 Important! - the uppercase versions of the above represent the complement of
the class. eg. %U represents everything except uppercase letters, %D represents
everything except digits.

 There are some "magic characters" (such as %) that have special meanings. These
are:


 ^ $ ( ) % . [ ] * + - ?


 If you want to use those in a pattern (as themselves) you must precede them by
a % symbol.

 eg. %% would match a single %

 You can build your own pattern classes (sets) by using square brackets, eg.


 [abc] ---> matches a, b or c
 [a-z] ---> matches lowercase letters (same as %l)
 [^abc] ---> matches anything except a, b or c
 [%a%d] ---> matches all letters and digits

 [%a%d_] ---> matches all letters, digits and underscore
 [%[%]] ---> matches square brackets (had to escape them with %)


 You can use pattern classes in the form %x in the set.
 If you use other characters (like periods and brackets, etc.) they are simply
themselves.

 You can specify a range of character inside a set by using simple characters
(not pattern classes like %a) separated by a hyphen. For example, [A-Z] or
[0-9]. These can be combined with other things. For example [A-Z0-9] or [A-Z,.].

 A end-points of a range must be given in ascending order. That is, [A-Z] would
match upper-case letters, but [Z-A] would not match anything.

 You can negate a set by starting it with a "^" symbol, thus [^0-9] is
everything except the digits 0 to 9. The negation applies to the whole set, so
[^%a%d] would match anything except letters or digits. In anywhere except the
first position of a set, the "^" symbol is simply itself.

 Inside a set (that is a sequence delimited by square brackets) the only "magic"
characters are:

 ] ---> to end the set, unless preceded by %
 % ---> to introduce a character class (like %a), or magic character (like "]")
 ^ ---> in the first position only, to negate the set (eg. [^A-Z)
 - ---> between two characters, to specify a range (eg. [A-F])


 Thus, inside a set, characters like "." and "?" are just themselves.

 The repetition characters, which can follow a character, class or set, are:


 +  ---> 1 or more repetitions (greedy)
 *  ---> 0 or more repetitions (greedy)

 -  ---> 0 or more repetitions (non greedy)
 ?  ---> 0 or 1 repetition only


 A "greedy" match will match on as many characters as possible, a non-greedy one
will match on as few as possible.

 The standard "anchor" characters apply:


 ^  ---> anchor to start of subject string
 $  ---> anchor to end of subject string


 You can also use round brackets to specify "captures":


 You see (.*) here


 Here, whatever matches (.*) becomes the first pattern.

 You can also refer to matched substrings (captures) later on in an expression:

 eg. This would match:

 string = "You see dogs and dogs"
 regexp = "You see (.*) and %1"


 This example shows how you can look for a repetition of a word matched earlier,
whatever that word was ("dogs" in this case).

 As a special case, an empty capture string returns as the captured pattern, the
position of itself in the string. eg.

 string = "You see dogs and dogs"
 regexp = "You .* ()dogs .*"

 This would return a capture with an offset of 8, and a length of CAP_POSITION
(-2)

 Finally you can look for nested "balanced" things (such as parentheses) by
using %b, like this:


 string = "I see a (big fish (swimming) in the pond) here"
 regexp = "%b()"


 After %b you put 2 characters, which indicate the start and end of the balanced
pair. If it finds a nested version it keeps processing until we are back at the
top level. In this case the matching string was "(big fish (swimming) in the
pond)".
*/

#ifndef OCTINC_REGEXP_HPP
#  define OCTINC_REGEXP_HPP
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif
#  if __cplusplus < 201103L && !defined(nullptr)
#    define nullptr NULL
#  endif

const char REGEXP_ESC = '%';
const char *REGEXP_SPECIALS = "^$*+?.([%-";
const int CAP_POSITION = -2;
const int CAP_UNFINISHED = -1；
const int ERR_INVALID_CAPTURE_INDEX = -1;
const int ERR_INVALID_PATTERN_CAPTURE = -2;
const int ERR_MALFORMED_PATTERN_ENDS_WITH_ESCAPE = -3;
const int ERR_MALFORMED_PATTERN_ENDS_WITH_RH_SQUARE_BRACKET = -4;
const int ERR_MISSING_LH_SQUARE_BRACKET_AFTER_ESC_F = -7;
const int ERR_NO_TARGET_STRING = -8;
const int ERR_TOO_MANY_CAPTURES = -6;
const int ERR_UNBALANCED_PATTERN = -5;
const int MAXCAPTURES = 32;
const int REGEXP_MATCHED = 1;
const int REGEXP_NOMATCH = 0;

template <typename _Tp>
unsigned char uchar(_Tp x) {
    return (unsigned char)(x);
}
namespace Octinc {
class regexp;

typedef void (*GlobalMatchCallback)(const char *match,
                                    const unsigned int length,
                                    const regexp &ms);
typedef void (*GlobalReplaceCallback)(const char *match,
                                      const unsigned int length,
                                      const char *&replacement,
                                      unsigned int &replacement_length,
                                      const regexp &ms);

namespace detail {

static jmp_buf regexp___regexp_throw_error_return;
typedef unsigned char byte;

static byte __regexp_throw_error(const char err) {
    longjmp(regexp___regexp_throw_error_return, err);
    return 0;
}

int __regexp_match_class(int c, int cl) {
    int res;
    switch (tolower(cl)) {
        case 'a':
            res = isalpha(c);
            break;
        case 'c':
            res = iscntrl(c);
            break;
        case 'd':
            res = isdigit(c);
            break;
        case 'l':
            res = islower(c);
            break;
        case 'p':
            res = ispunct(c);
            break;
        case 's':
            res = isspace(c);
            break;
        case 'u':
            res = isupper(c);
            break;
        case 'w':
            res = isalnum(c);
            break;
        case 'x':
            res = isxdigit(c);
            break;
        case 'z':
            res = (c == 0);
            break;
        default:
            return (cl == c);
    }
    return (islower(cl) ? res : !res);
}

int __regexp_match_bracket_class(int c, const char *p, const char *ec) {
    int sig = 1;
    if (*(p + 1) == '^') {
        sig = 0;
        p++;
    }
    while (++p < ec) {
        if (*p == REGEXP_ESC) {
            p++;
            if (__regexp_match_class(c, uchar(*p))) return sig;
        } else if ((*(p + 1) == '-') && (p + 2 < ec)) {
            p += 2;
            if (uchar(*(p - 2)) <= c && c <= uchar(*p)) return sig;
        } else if (uchar(*p) == c)
            return sig;
    }
    return !sig;
}

int __regexp_single_match(int c, const char *p, const char *ep) {
    switch (*p) {
        case '.':
            return 1;
        case REGEXP_ESC:
            return __regexp_match_class(c, uchar(*(p + 1)));
        case '[':
            return __regexp_match_bracket_class(c, p, ep - 1);
        default:
            return (uchar(*p) == c);
    }
}
}  // namespace detail

class regexp {
   private:
    char result;

   public:
    regexp() : result(REGEXP_NOMATCH), src(0) {}
    regexp(char *s) : result(REGEXP_NOMATCH) { insert(s); }
    regexp(char *s, const unsigned int len) : result(REGEXP_NOMATCH) {
        insert(s, len);
    }

   protected:
    char *src;
    unsigned int src_len;

    char *src_end;

    unsigned int MatchStart;
    unsigned int MatchLength;

    int level;

    struct {
        const char *init;
        int len;
    } capture[MAXCAPTURES];

   public:
    unsigned int start()
    /*
    zero-relative offset of start of match.
    */
    {
        return MatchStart;
    }

    unsigned int size()
    /*
    length of match.
    */
    {
        return MatchLength;
    }

    void insert(char *);
    /*
    Add target string, null-terminated.
    */
    void insert(char *, const unsigned int);
    /*
    Add target string, with specified length.
    */
    char match(const char *, unsigned int index = 0);
    /*
    Do a match on a supplied pattern and zero-relative starting point.
    */
    char *query_match(char *) const;
    /*
    Return the matching string.
    */
    char *previous_match(char *, const int) const;
    /*
    Return capture string n.
    */
    char query() const { return result; }

    unsigned int count(const char *);
    /*
    Count number of matches on a supplied pattern.
    */
    unsigned int global_match(const char *, GlobalMatchCallback);
    /*
    Iterate with a supplied pattern, call function f for each match,
    and returns count of matches.
    */
    unsigned int replace(const char *, GlobalReplaceCallback,
                         const unsigned int max_count = 0);
    /*
    Iterate with a supplied pattern, call function f for each match, maximum of
    max_count matches if max_count > 0, returns count of replacements.
    */
    unsigned int replace(const char *, const char *,
                         const unsigned int max_count = 0);
    /*
    Iterate with a supplied pattern, replaces with replacement string, maximum
    of max_count matches if max_count > 0, returns count of replacements.
    */
   private:
    friend int check_capture(regexp *ms, int l) {
        l -= '1';
        if (l < 0 || l >= ms->level || ms->capture[l].len == CAP_UNFINISHED)
            return detail::__regexp_throw_error(ERR_INVALID_CAPTURE_INDEX);
        return l;
    }

    friend int capture_to_close(regexp *ms) {
        int level = ms->level;
        for (level--; level >= 0; level--)
            if (ms->capture[level].len == CAP_UNFINISHED) return level;
        return detail::__regexp_throw_error(ERR_INVALID_PATTERN_CAPTURE);
    }

    friend const char *classend(regexp *ms, const char *p) {
        switch (*p++) {
            case REGEXP_ESC: {
                if (*p == '\0')
                    detail::__regexp_throw_error(
                        ERR_MALFORMED_PATTERN_ENDS_WITH_ESCAPE);
                return p + 1;
            }
            case '[': {
                if (*p == '^') p++;
                do {
                    if (*p == '\0')
                        detail::__regexp_throw_error(
                            ERR_MALFORMED_PATTERN_ENDS_WITH_RH_SQUARE_BRACKET);
                    if (*(p++) == REGEXP_ESC && *p != '\0') p++;
                } while (*p != ']');
                return p + 1;
            }
            default: { return p; }
        }
    }

    friend const char *str_match(regexp *ms, const char *s, const char *p);

    friend const char *matchbalance(regexp *ms, const char *s, const char *p) {
        if (*p == 0 || *(p + 1) == 0)
            detail::__regexp_throw_error(ERR_UNBALANCED_PATTERN);
        if (*s != *p)
            return nullptr;
        else {
            int b = *p;
            int e = *(p + 1);
            int cont = 1;
            while (++s < ms->src_end) {
                if (*s == e) {
                    if (--cont == 0) return s + 1;
                } else if (*s == b)
                    cont++;
            }
        }
        return nullptr;
    }

    friend const char *max_expand(regexp *ms, const char *s, const char *p,
                                  const char *ep) {
        int i = 0;
        while ((s + i) < ms->src_end &&
               detail::__regexp_single_match(uchar(*(s + i)), p, ep))
            i++;

        while (i >= 0) {
            const char *res = str_match(ms, (s + i), ep + 1);
            if (res) return res;
            i--;
        }
        return nullptr;
    }

    friend const char *min_expand(regexp *ms, const char *s, const char *p,
                                  const char *ep) {
        for (;;) {
            const char *res = str_match(ms, s, ep + 1);
            if (res != nullptr)
                return res;
            else if (s < ms->src_end &&
                     detail::__regexp_single_match(uchar(*s), p, ep))
                s++;
            else
                return nullptr;
        }
    }

    friend const char *start_capture(regexp *ms, const char *s, const char *p,
                                     int what) {
        const char *res;
        int level = ms->level;
        if (level >= MAXCAPTURES)
            detail::__regexp_throw_error(ERR_TOO_MANY_CAPTURES);
        ms->capture[level].init = s;
        ms->capture[level].len = what;
        ms->level = level + 1;
        if ((res = str_match(ms, s, p)) == nullptr) ms->level--;
        return res;
    }

    friend const char *end_capture(regexp *ms, const char *s, const char *p) {
        int l = capture_to_close(ms);
        const char *res;
        ms->capture[l].len = s - ms->capture[l].init;
        if ((res = str_match(ms, s, p)) == nullptr)
            ms->capture[l].len = CAP_UNFINISHED;
        return res;
    }

    friend const char *match_capture(regexp *ms, const char *s, int l) {
        size_t len;
        l = check_capture(ms, l);
        len = ms->capture[l].len;
        if ((size_t)(ms->src_end - s) >= len &&
            memcmp(ms->capture[l].init, s, len) == 0)
            return s + len;
        else
            return nullptr;
    }

    friend const char *str_match(regexp *ms, const char *s, const char *p) {
    init:
        switch (*p) {
            case '(': {
                if (*(p + 1) == ')')
                    return start_capture(ms, s, p + 2, CAP_POSITION);
                else
                    return start_capture(ms, s, p + 1, CAP_UNFINISHED);
            }
            case ')': {
                return end_capture(ms, s, p + 1);
            }
            case REGEXP_ESC: {
                switch (*(p + 1)) {
                    case 'b': {
                        s = matchbalance(ms, s, p + 2);
                        if (s == nullptr) return nullptr;
                        p += 4;
                        goto init;
                    }
                    case 'f': {
                        const char *ep;
                        char previous;
                        p += 2;
                        if (*p != '[')
                            detail::__regexp_throw_error(
                                ERR_MISSING_LH_SQUARE_BRACKET_AFTER_ESC_F);
                        ep = classend(ms, p);
                        previous = (s == ms->src) ? '\0' : *(s - 1);
                        if (detail::__regexp_match_bracket_class(
                                uchar(previous), p, ep - 1) ||
                            !detail::__regexp_match_bracket_class(uchar(*s), p,
                                                                  ep - 1))
                            return nullptr;
                        p = ep;
                        goto init;
                    }
                    default: {
                        if (isdigit(uchar(*(p + 1)))) {
                            s = match_capture(ms, s, uchar(*(p + 1)));
                            if (s == nullptr) return nullptr;
                            p += 2;
                            goto init;
                        }
                        goto dflt;
                    }
                }
            }
            case '\0': {
                return s;
            }
            case '$': {
                if (*(p + 1) == '\0')
                    return (s == ms->src_end) ? s : nullptr;
                else
                    goto dflt;
            }
            default:
            dflt : {
                const char *ep = classend(ms, p);
                int m = s < ms->src_end &&
                        detail::__regexp_single_match(uchar(*s), p, ep);
                switch (*ep) {
                    case '?': {
                        const char *res;
                        if (m &&
                            ((res = str_match(ms, s + 1, ep + 1)) != nullptr))
                            return res;
                        p = ep + 1;
                        goto init;
                    }
                    case '*': {
                        return max_expand(ms, s, p, ep);
                    }
                    case '+': {
                        return (m ? max_expand(ms, s + 1, p, ep) : nullptr);
                    }
                    case '-': {
                        return min_expand(ms, s, p, ep);
                    }
                    default: {
                        if (!m) return nullptr;
                        s++;
                        p = ep;
                        goto init;
                    }
                }
            }
        }
    }
};

char regexp::match(const char *pattern, unsigned int index) {
    char rtn = setjmp(detail::regexp___regexp_throw_error_return);

    if (rtn) return ((result = rtn));

    if (!src) detail::__regexp_throw_error(ERR_NO_TARGET_STRING);

    if (index > src_len) index = src_len;

    int anchor = (*pattern == '^') ? (pattern++, 1) : 0;
    const char *s1 = src + index;
    src_end = src + src_len;

    do {
        const char *res;
        level = 0;
        if ((res = str_match(this, s1, pattern)) != nullptr) {
            MatchStart = s1 - src;
            MatchLength = res - s1;
            return (result = REGEXP_MATCHED);
        }
    } while (s1++ < src_end && !anchor);

    return (result = REGEXP_NOMATCH);
}

void regexp::insert(char *s) { insert(s, strlen(s)); }

void regexp::insert(char *s, const unsigned int len) {
    src = s;
    src_len = len;
    result = REGEXP_NOMATCH;
}

char *regexp::query_match(char *s) const {
    if (result != REGEXP_MATCHED)
        s[0] = 0;
    else {
        memcpy(s, &src[MatchStart], MatchLength);
        s[MatchLength] = 0;
    }
    return s;
}

char *regexp::previous_match(char *s, const int n) const {
    if (result != REGEXP_MATCHED || n >= level || capture[n].len <= 0)
        s[0] = 0;
    else {
        memcpy(s, capture[n].init, capture[n].len);
        s[capture[n].len] = 0;
    }
    return s;
}

unsigned int regexp::count(const char *pattern) {
    unsigned int count = 0;

    for (unsigned int index = 0; match(pattern, index) > 0 && index < src_len;
         count++)

        index = MatchStart + (MatchLength == 0 ? 1 : MatchLength);

    return count;
}

unsigned int regexp::global_match(const char *pattern, GlobalMatchCallback f) {
    unsigned int count = 0;

    for (unsigned int index = 0; match(pattern, index) > 0; count++) {
        f(&src[MatchStart], MatchLength, *this);

        index = MatchStart + (MatchLength == 0 ? 1 : MatchLength);
    }
    return count;
}

unsigned int regexp::replace(const char *pattern, GlobalReplaceCallback f,
                             const unsigned int max_count) {
    unsigned int count = 0;

    for (unsigned int index = 0; match(pattern, index) > 0 && index < src_len &&
                                 (max_count == 0 || count < max_count);
         count++) {
        const char *replacement = &src[MatchStart];
        unsigned int replacement_length = MatchLength;

        if (MatchLength == 0)
            index = MatchStart + 1;
        else {
            index = MatchStart + MatchLength;

            f(&src[MatchStart], MatchLength, replacement, replacement_length,
              *this);

            int lengthDiff = MatchLength - replacement_length;

            memmove(&src[index - lengthDiff], &src[index], src_len - index);

            memmove(&src[MatchStart], replacement, replacement_length);

            index -= lengthDiff;

            src_len -= lengthDiff;
        }
    }

    src[src_len] = 0;
    return count;
}

unsigned int regexp::replace(const char *pattern, const char *replacement,
                             const unsigned int max_count) {
    unsigned int count = 0;
    unsigned int replacement_length = strlen(replacement);

    for (unsigned int index = 0; match(pattern, index) > 0 && index < src_len &&
                                 (max_count == 0 || count < max_count);
         count++) {
        if (MatchLength == 0)
            index = MatchStart + 1;
        else {
            index = MatchStart + MatchLength;

            int lengthDiff = MatchLength - replacement_length;

            memmove(&src[index - lengthDiff], &src[index], src_len - index);

            memmove(&src[MatchStart], replacement, replacement_length);

            index -= lengthDiff;

            src_len -= lengthDiff;
        }
    }

    src[src_len] = 0;
    return count;
}
}  // namespace Octinc

#endif
