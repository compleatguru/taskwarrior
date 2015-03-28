////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013 - 2015, Göteborg Bit Factory.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// http://www.opensource.org/licenses/mit-license.php
//
////////////////////////////////////////////////////////////////////////////////

#include <cmake.h>
#include <iostream>
#include <vector>
#include <test.h>
#include <Lexer.h>
#include <Context.h>

Context context;

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char** argv)
{
  UnitTest t (229);

  std::vector <std::pair <std::string, Lexer::Type> > tokens;
  std::string token;
  Lexer::Type type;

  // White space detection.
  t.notok (Lexer::isWhitespace (0x0041), "U+0041 (A) ! isWhitespace");
  t.ok (Lexer::isWhitespace (0x0020), "U+0020 isWhitespace");
  t.ok (Lexer::isWhitespace (0x0009), "U+0009 isWhitespace");
  t.ok (Lexer::isWhitespace (0x000A), "U+000A isWhitespace");
  t.ok (Lexer::isWhitespace (0x000B), "U+000B isWhitespace");
  t.ok (Lexer::isWhitespace (0x000C), "U+000C isWhitespace");
  t.ok (Lexer::isWhitespace (0x000D), "U+000D isWhitespace");
  t.ok (Lexer::isWhitespace (0x0085), "U+0085 isWhitespace");
  t.ok (Lexer::isWhitespace (0x00A0), "U+00A0 isWhitespace");
  t.ok (Lexer::isWhitespace (0x1680), "U+1680 isWhitespace"); // 10
  t.ok (Lexer::isWhitespace (0x180E), "U+180E isWhitespace");
  t.ok (Lexer::isWhitespace (0x2000), "U+2000 isWhitespace");
  t.ok (Lexer::isWhitespace (0x2001), "U+2001 isWhitespace");
  t.ok (Lexer::isWhitespace (0x2002), "U+2002 isWhitespace");
  t.ok (Lexer::isWhitespace (0x2003), "U+2003 isWhitespace");
  t.ok (Lexer::isWhitespace (0x2004), "U+2004 isWhitespace");
  t.ok (Lexer::isWhitespace (0x2005), "U+2005 isWhitespace");
  t.ok (Lexer::isWhitespace (0x2006), "U+2006 isWhitespace");
  t.ok (Lexer::isWhitespace (0x2007), "U+2007 isWhitespace");
  t.ok (Lexer::isWhitespace (0x2008), "U+2008 isWhitespace"); // 20
  t.ok (Lexer::isWhitespace (0x2009), "U+2009 isWhitespace");
  t.ok (Lexer::isWhitespace (0x200A), "U+200A isWhitespace");
  t.ok (Lexer::isWhitespace (0x2028), "U+2028 isWhitespace");
  t.ok (Lexer::isWhitespace (0x2029), "U+2029 isWhitespace");
  t.ok (Lexer::isWhitespace (0x202F), "U+202F isWhitespace");
  t.ok (Lexer::isWhitespace (0x205F), "U+205F isWhitespace");
  t.ok (Lexer::isWhitespace (0x3000), "U+3000 isWhitespace");

  // static bool Lexer::isBoundary(int, int);
  t.ok    (Lexer::isBoundary (' ', 'a'), "' ' --> 'a' = isBoundary");
  t.ok    (Lexer::isBoundary ('a', ' '), "'a' --> ' ' = isBoundary");
  t.ok    (Lexer::isBoundary (' ', '+'), "' ' --> '+' = isBoundary");
  t.ok    (Lexer::isBoundary (' ', ','), "' ' --> ',' = isBoundary");
  t.notok (Lexer::isBoundary ('3', '4'), "'3' --> '4' = isBoundary");
  t.ok    (Lexer::isBoundary ('(', '('), "'(' --> '(' = isBoundary");
  t.notok (Lexer::isBoundary ('r', 'd'), "'r' --> 'd' = isBoundary");

  // Should result in no tokens.
  Lexer l0 ("");
  t.notok (l0.token (token, type), "'' --> no tokens");

  // Should result in no tokens.
  Lexer l1 ("       \t ");
  t.notok (l1.token (token, type), "'       \\t ' --> no tokens");

  // \u20ac = Euro symbol.
  Lexer l2 (" one 'two \\'three\\''+456-(1.3*2 - 0x12) 1.2e-3.4    foo.bar and '\\u20ac'");

  tokens.clear ();
  while (l2.token (token, type))
  {
    std::cout << "# «" << token << "» " << Lexer::typeName (type) << "\n";
    tokens.push_back (std::pair <std::string, Lexer::Type> (token, type));
  }

  t.is (tokens[0].first,                     "one",           "tokens[0] = 'one'"); // 30
  t.is (Lexer::typeName (tokens[0].second),  "dom",           "tokens[0] = dom");

  t.is (tokens[1].first,                     "'two 'three''", "tokens[1] = 'two 'three''");
  t.is (Lexer::typeName (tokens[1].second),  "string",        "tokens[1] = string");

  t.is (tokens[2].first,                     "+",             "tokens[2] = '+'");
  t.is (Lexer::typeName (tokens[2].second),  "op",            "tokens[2] = op");

  t.is (tokens[3].first,                     "456",           "tokens[3] = '456'");
  t.is (Lexer::typeName (tokens[3].second),  "number",        "tokens[3] = number");

  t.is (tokens[4].first,                     "-",             "tokens[4] = '-'");
  t.is (Lexer::typeName (tokens[4].second),  "op",            "tokens[4] = op");

  t.is (tokens[5].first,                     "(",             "tokens[5] = '('"); // 40
  t.is (Lexer::typeName (tokens[5].second),  "op",            "tokens[5] = op");

  t.is (tokens[6].first,                     "1.3",           "tokens[6] = '1.3'");
  t.is (Lexer::typeName (tokens[6].second),  "number",        "tokens[6] = number");

  t.is (tokens[7].first,                     "*",             "tokens[7] = '*'");
  t.is (Lexer::typeName (tokens[7].second),  "op",            "tokens[7] = op");

  t.is (tokens[8].first,                     "2",             "tokens[8] = '2'");
  t.is (Lexer::typeName (tokens[8].second),  "number",        "tokens[8] = number");

  t.is (tokens[9].first,                     "-",             "tokens[9] = '-'");
  t.is (Lexer::typeName (tokens[9].second),  "op",            "tokens[9] = op");

  t.is (tokens[10].first,                    "0x12",          "tokens[10] = '0x12'"); // 50
  t.is (Lexer::typeName (tokens[10].second), "hex",           "tokens[10] = hex");

  t.is (tokens[11].first,                    ")",             "tokens[11] = ')'");
  t.is (Lexer::typeName (tokens[11].second), "op",            "tokens[11] = op");

  t.is (tokens[12].first,                    "1.2e-3.4",      "tokens[12] = '1.2e-3.4'");
  t.is (Lexer::typeName (tokens[12].second), "number",        "tokens[12] = number");

  t.is (tokens[13].first,                    "foo.bar",       "tokens[13] = 'foo.bar'");
  t.is (Lexer::typeName (tokens[13].second), "dom",           "tokens[13] = dom");

  t.is (tokens[14].first,                    "and",           "tokens[14] = 'and'"); // 60
  t.is (Lexer::typeName (tokens[14].second), "op",            "tokens[14] = op");

  t.is (tokens[15].first,                    "'€'",           "tokens[15] = \\u20ac --> ''€''");
  t.is (Lexer::typeName (tokens[15].second), "string",        "tokens[15] = string");

  // Test for ISO-8601 dates (favoring dates in ambiguous cases).
  Lexer l3 ("1 12 123 1234 12345 123456 1234567 12345678 20131129T225800Z 2013-11-29T22:58:00Z");
  l3.ambiguity (true);
  tokens.clear ();
  while (l3.token (token, type))
  {
    std::cout << "# «" << token << "» " << Lexer::typeName (type) << "\n";
    tokens.push_back (std::pair <std::string, Lexer::Type> (token, type));
  }

  t.is ((int)tokens.size (),     10,                        "10 tokens");
  t.is (tokens[0].first,         "1",                       "tokens[0] == '1'");
  t.is ((int) tokens[0].second,  (int) Lexer::Type::number, "tokens[0] == Type::number");
  t.is (tokens[1].first,         "12",                      "tokens[1] == '12'");
  t.is ((int) tokens[1].second,  (int) Lexer::Type::date,   "tokens[1] == Type::date");
  t.is (tokens[2].first,         "123",                     "tokens[2] == '123'");
  t.is ((int) tokens[2].second,  (int) Lexer::Type::number, "tokens[2] == Type::number"); // 70
  t.is (tokens[3].first,         "1234",                    "tokens[3] == '1234'");
  t.is ((int) tokens[3].second,  (int) Lexer::Type::date,   "tokens[3] == Type::date");
  t.is (tokens[4].first,         "12345",                   "tokens[4] == '12345'");
  t.is ((int) tokens[4].second,  (int) Lexer::Type::number, "tokens[4] == Type::number");
  t.is (tokens[5].first,         "123456",                  "tokens[5] == '123456'");
  t.is ((int) tokens[5].second,  (int) Lexer::Type::date,   "tokens[5] == Type::date");
  t.is (tokens[6].first,         "1234567",                 "tokens[6] == '1234567'");
  t.is ((int) tokens[6].second,  (int) Lexer::Type::number, "tokens[6] == Type::number");
  t.is (tokens[7].first,         "12345678",                "tokens[7] == '12345678'");
  t.is ((int) tokens[7].second,  (int) Lexer::Type::number, "tokens[7] == Type::number"); // 80
  t.is (tokens[8].first,         "20131129T225800Z",        "tokens[8] == '20131129T225800Z'");
  t.is ((int) tokens[8].second,  (int) Lexer::Type::date,   "tokens[8] == Type::date");
  t.is (tokens[9].first,         "2013-11-29T22:58:00Z",    "tokens[9] == '2013-11-29T22:58:00Z'");
  t.is ((int) tokens[9].second,  (int) Lexer::Type::date,   "tokens[9] == Type::date");

  // Test for ISO-8601 dates (favoring numbers in ambiguous cases).
  Lexer l4 ("1 12 123 1234 12345 123456 1234567 12345678 20131129T225800Z 2013-11-29T22:58:00Z");
  l4.ambiguity (false);
  tokens.clear ();
  while (l4.token (token, type))
  {
    std::cout << "# «" << token << "» " << Lexer::typeName (type) << "\n";
    tokens.push_back (std::pair <std::string, Lexer::Type> (token, type));
  }

  t.is ((int)tokens.size (),       10,                          "10 tokens");
  t.is (tokens[0].first,           "1",                         "tokens[0] == '1'");
  t.is ((int) tokens[0].second,    (int) Lexer::Type::number,   "tokens[0] == Type::number");
  t.is (tokens[1].first,           "12",                        "tokens[1] == '12'");
  t.is ((int) tokens[1].second,    (int) Lexer::Type::number,   "tokens[1] == Type::number");
  t.is (tokens[2].first,           "123",                       "tokens[2] == '123'"); // 90
  t.is ((int) tokens[2].second,    (int) Lexer::Type::number,   "tokens[2] == Type::number");
  t.is (tokens[3].first,           "1234",                      "tokens[3] == '1234'");
  t.is ((int) tokens[3].second,    (int) Lexer::Type::number,   "tokens[3] == Type::number");
  t.is (tokens[4].first,           "12345",                     "tokens[4] == '12345'");
  t.is ((int) tokens[4].second,    (int) Lexer::Type::number,   "tokens[4] == Type::number");
  t.is (tokens[5].first,           "123456",                    "tokens[5] == '123456'");
  t.is ((int) tokens[5].second,    (int) Lexer::Type::number,   "tokens[5] == Type::number");
  t.is (tokens[6].first,           "1234567",                   "tokens[6] == '1234567'");
  t.is ((int) tokens[6].second,    (int) Lexer::Type::number,   "tokens[6] == Type::number");
  t.is (tokens[7].first,           "12345678",                  "tokens[7] == '12345678'"); // 100
  t.is ((int) tokens[7].second,    (int) Lexer::Type::number,   "tokens[7] == Type::number");
  t.is (tokens[8].first,           "20131129T225800Z",          "tokens[8] == '20131129T225800Z'");
  t.is ((int) tokens[8].second,    (int) Lexer::Type::date,     "tokens[8] == Type::date");
  t.is (tokens[9].first,           "2013-11-29T22:58:00Z",      "tokens[9] == '2013-11-29T22:58:00Z'");
  t.is ((int) tokens[9].second,    (int) Lexer::Type::date,     "tokens[9] == Type::date");

  // Test for durations
  Lexer l5 ("1second 1minute 2hour 3 days 4w 5mo 6 years");
  tokens.clear ();
  while (l5.token (token, type))
  {
    std::cout << "# «" << token << "» " << Lexer::typeName (type) << "\n";
    tokens.push_back (std::pair <std::string, Lexer::Type> (token, type));
  }

  t.is ((int)tokens.size (),       7,                           "7 tokens");
  t.is (tokens[0].first,           "1second",                   "tokens[0] == '1second'");
  t.is ((int) tokens[0].second,    (int) Lexer::Type::duration, "tokens[0] == Type::duration");
  t.is (tokens[1].first,           "1minute",                   "tokens[1] == '1minute'");
  t.is ((int) tokens[1].second,    (int) Lexer::Type::duration, "tokens[1] == Type::duration"); // 110
  t.is (tokens[2].first,           "2hour",                     "tokens[2] == '2hour'");
  t.is ((int) tokens[2].second,    (int) Lexer::Type::duration, "tokens[2] == Type::duration");
  t.is (tokens[3].first,           "3 days",                    "tokens[3] == '3 days'");
  t.is ((int) tokens[3].second,    (int) Lexer::Type::duration, "tokens[3] == Type::duration");
  t.is (tokens[4].first,           "4w",                        "tokens[4] == '4w'");
  t.is ((int) tokens[4].second,    (int) Lexer::Type::duration, "tokens[4] == Type::duration");
  t.is (tokens[5].first,           "5mo",                       "tokens[5] == '5mo'");
  t.is ((int) tokens[5].second,    (int) Lexer::Type::duration, "tokens[5] == Type::duration");
  t.is (tokens[6].first,           "6 years",                   "tokens[6] == '6 years'");
  t.is ((int) tokens[6].second,    (int) Lexer::Type::duration, "tokens[6] == Type::duration"); // 120

  // All the Eval operators.
  Lexer l6 ("P1Y PT1H P1Y1M1DT1H1M1S 1s 1second");
  tokens.clear ();
  while (l6.token (token, type))
  {
    std::cout << "# «" << token << "» " << Lexer::typeName (type) << "\n";
    tokens.push_back (std::pair <std::string, Lexer::Type> (token, type));
  }

  t.is ((int)tokens.size (),       5,                           "5 ISO periods");
  t.is (tokens[0].first,           "P1Y",                       "tokens[0] == 'P1Y'");
  t.is ((int) tokens[0].second,    (int) Lexer::Type::duration, "tokens[0] == Type::duration");
  t.is (tokens[1].first,           "PT1H",                      "tokens[1] == 'PT1H'");
  t.is ((int) tokens[1].second,    (int) Lexer::Type::duration, "tokens[1] == Type::duration");
  t.is (tokens[2].first,           "P1Y1M1DT1H1M1S",            "tokens[2] == 'P1Y1M1DT1H1M1S'");
  t.is ((int) tokens[2].second,    (int) Lexer::Type::duration, "tokens[2] == Type::duration");
  t.is (tokens[3].first,           "1s",                        "tokens[3] == '1s'");
  t.is ((int) tokens[3].second,    (int) Lexer::Type::duration, "tokens[3] == Type::duration");
  t.is (tokens[4].first,           "1second",                   "tokens[4] == '1second'");
  t.is ((int) tokens[4].second,    (int) Lexer::Type::duration, "tokens[4] == Type::duration");

  // All (int) the Eval operators.
  Lexer l7 ("and xor or <= >= !~ != == = ^ > ~ ! * / % + - < ( )");
  tokens.clear ();
  while (l7.token (token, type))
  {
    std::cout << "# «" << token << "» " << Lexer::typeName (type) << "\n";
    tokens.push_back (std::pair <std::string, Lexer::Type> (token, type));
  }

  t.is ((int)tokens.size (),       21,                    "21 operators");
  t.is (tokens[0].first,           "and",                 "tokens[0] == 'and'");
  t.is ((int) tokens[0].second,    (int) Lexer::Type::op, "tokens[0] == Type::op"); // 130
  t.is (tokens[1].first,           "xor",                 "tokens[1] == 'xor'");
  t.is ((int) tokens[1].second,    (int) Lexer::Type::op, "tokens[1] == Type::op");
  t.is (tokens[2].first,           "or",                  "tokens[2] == 'or'");
  t.is ((int) tokens[2].second,    (int) Lexer::Type::op, "tokens[2] == Type::op");
  t.is (tokens[3].first,           "<=",                  "tokens[3] == '<='");
  t.is ((int) tokens[3].second,    (int) Lexer::Type::op, "tokens[3] == Type::op");
  t.is (tokens[4].first,           ">=",                  "tokens[4] == '>='");
  t.is ((int) tokens[4].second,    (int) Lexer::Type::op, "tokens[4] == Type::op");
  t.is (tokens[5].first,           "!~",                  "tokens[5] == '!~'");
  t.is ((int) tokens[5].second,    (int) Lexer::Type::op, "tokens[5] == Type::op"); // 140
  t.is (tokens[6].first,           "!=",                  "tokens[6] == '!='");
  t.is ((int) tokens[6].second,    (int) Lexer::Type::op, "tokens[6] == Type::op");
  t.is (tokens[7].first,           "==",                  "tokens[7] == '=='");
  t.is ((int) tokens[7].second,    (int) Lexer::Type::op, "tokens[7] == Type::op");
  t.is (tokens[8].first,           "=",                   "tokens[8] == '='");
  t.is ((int) tokens[8].second,    (int) Lexer::Type::op, "tokens[8] == Type::op");
  t.is (tokens[9].first,           "^",                   "tokens[9] == '^'");
  t.is ((int) tokens[9].second,    (int) Lexer::Type::op, "tokens[9] == Type::op");
  t.is (tokens[10].first,          ">",                   "tokens[10] == '>'");
  t.is ((int) tokens[10].second,   (int) Lexer::Type::op, "tokens[10] == Type::op"); // 150
  t.is (tokens[11].first,          "~",                   "tokens[11] == '~'");
  t.is ((int) tokens[11].second,   (int) Lexer::Type::op, "tokens[11] == Type::op");
  t.is (tokens[12].first,          "!",                   "tokens[12] == '!'");
  t.is ((int) tokens[12].second,   (int) Lexer::Type::op, "tokens[12] == Type::op");
  t.is (tokens[13].first,          "*",                   "tokens[13] == '*'");
  t.is ((int) tokens[13].second,   (int) Lexer::Type::op, "tokens[13] == Type::op");
  t.is (tokens[14].first,          "/",                   "tokens[14] == '/'");
  t.is ((int) tokens[14].second,   (int) Lexer::Type::op, "tokens[14] == Type::op");
  t.is (tokens[15].first,          "%",                   "tokens[15] == '%'");
  t.is ((int) tokens[15].second,   (int) Lexer::Type::op, "tokens[15] == Type::op"); // 160
  t.is (tokens[16].first,          "+",                   "tokens[16] == '+'");
  t.is ((int) tokens[16].second,   (int) Lexer::Type::op, "tokens[16] == Type::op");
  t.is (tokens[17].first,          "-",                   "tokens[17] == '-'");
  t.is ((int) tokens[17].second,   (int) Lexer::Type::op, "tokens[17] == Type::op");
  t.is (tokens[18].first,          "<",                   "tokens[18] == '<'");
  t.is ((int) tokens[18].second,   (int) Lexer::Type::op, "tokens[18] == Type::op");
  t.is (tokens[19].first,          "(",                   "tokens[19] == '('");
  t.is ((int) tokens[19].second,   (int) Lexer::Type::op, "tokens[19] == Type::op");
  t.is (tokens[20].first,          ")",                   "tokens[20] == ')'");
  t.is ((int) tokens[20].second,   (int)Lexer::Type::op,  "tokens[20] == Type::op"); // 170

  // Test ordinal dates.
  Lexer l8 ("9th 10th");
  l8.ambiguity (false);
  tokens.clear ();
  while (l8.token (token, type))
  {
    std::cout << "# «" << token << "» " << Lexer::typeName (type) << "\n";
    tokens.push_back (std::pair <std::string, Lexer::Type> (token, type));
  }

  t.is ((int)tokens.size (),       2,                       "2 tokens");
  t.is (tokens[0].first,           "9th",                   "tokens[0] == '9th'");
  t.is ((int) tokens[0].second,    (int) Lexer::Type::word, "tokens[0] == Type::word");
  t.is (tokens[1].first,           "10th",                  "tokens[1] == '10th'");
  t.is ((int) tokens[1].second,    (int) Lexer::Type::word, "tokens[1] == Type::word");

  // Test tag recognition.
  Lexer l9 ("+with -WITHOUT + 2");
  l9.ambiguity (false);
  tokens.clear ();
  while (l9.token (token, type))
  {
    std::cout << "# «" << token << "» " << Lexer::typeName (type) << "\n";
    tokens.push_back (std::pair <std::string, Lexer::Type> (token, type));
  }

  t.is ((int)tokens.size (),       4,                         "4 tokens");
  t.is (tokens[0].first,           "+with",                   "tokens[0] == '+with'");
  t.is ((int) tokens[0].second,    (int) Lexer::Type::tag,    "tokens[0] == Type::tag");
  t.is (tokens[1].first,           "-WITHOUT",                "tokens[1] == '-WITHOUT'");
  t.is ((int) tokens[1].second,    (int) Lexer::Type::tag,    "tokens[1] == Type::tag");
  t.is (tokens[2].first,           "+",                       "tokens[2] == '+'");
  t.is ((int) tokens[2].second,    (int) Lexer::Type::op,     "tokens[2] == Type::op");
  t.is (tokens[3].first,           "2",                       "tokens[3] == '2'");
  t.is ((int) tokens[3].second,    (int) Lexer::Type::number, "tokens[3] == Type::number");

  // void split (std::vector<std::string>&, const std::string&);
  std::string unsplit = " ( A or B ) ";
  std::vector <std::string> items;
  items = Lexer::split (unsplit);
  t.is (items.size (), (size_t) 5, "split ' ( A or B ) '");
  t.is (items[0], "(",             "split ' ( A or B ) ' -> [0] '('");
  t.is (items[1], "A",             "split ' ( A or B ) ' -> [1] 'A'");
  t.is (items[2], "or",            "split ' ( A or B ) ' -> [2] 'or'");
  t.is (items[3], "B",             "split ' ( A or B ) ' -> [3] 'B'");
  t.is (items[4], ")",             "split ' ( A or B ) ' -> [4] ')'");

  // Test simple mode with contrived tokens that ordinarily split.
  unsplit = "  +-* a+b 12.3e4 'c d'";
  items = Lexer::split (unsplit);
  t.is (items.size (), (size_t) 8, "split '  +-* a+b 12.3e4 'c d''");
  t.is (items[0], "+",             "split '  +-* a+b 12.3e4 'c d'' -> [0] '+'");
  t.is (items[1], "-",             "split '  +-* a+b 12.3e4 'c d'' -> [1] '-'");
  t.is (items[2], "*",             "split '  +-* a+b 12.3e4 'c d'' -> [2] '*'");
  t.is (items[3], "a",             "split '  +-* a+b 12.3e4 'c d'' -> [3] 'a'");
  t.is (items[4], "+",             "split '  +-* a+b 12.3e4 'c d'' -> [4] '+'");
  t.is (items[5], "b",             "split '  +-* a+b 12.3e4 'c d'' -> [5] 'b'");
  t.is (items[6], "12.3e4",        "split '  +-* a+b 12.3e4 'c d'' -> [6] '12.3e4'");
  t.is (items[7], "'c d'",         "split '  +-* a+b 12.3e4 'c d'' -> [7] ''c d''");

  // Test common expression element.
  unsplit = "name=value";
  items = Lexer::split (unsplit);
  t.is (items.size (), (size_t) 3, "split 'name=value'");
  if (items.size () == 3)
  {
    t.is (items[0], "name",        "split 'name=value' -> [0] 'name'");
    t.is (items[1], "=",           "split 'name=value' -> [1] '='");
    t.is (items[2], "value",       "split 'name=value' -> [2] 'value'");
  }
  else
  {
    t.fail ("split 'name=value' -> [0] 'name'");
    t.fail ("split 'name=value' -> [1] '='");
    t.fail ("split 'name=value' -> [2] 'value'");
  }

  // Test unterminated tokens.
  unsplit = " ordinary ";
  items = Lexer::split (unsplit);
  t.is (items.size (), (size_t) 1, "split 'ordinary' --> 1 token");
  t.is (items[0], "ordinary",      "split 'ordinary' --> 'ordinary'");

  // Test recognized lexeme types.
  Lexer l10 ("/foo/ "
             "/a\\/b/ "
             "/'/ "
             "desc~pattern "
             "desc.cont:pattern ");
  l10.ambiguity (false);
  tokens.clear ();
  while (l10.token (token, type))
  {
    std::cout << "# «" << token << "» " << Lexer::typeName (type) << "\n";
    tokens.push_back (std::pair <std::string, Lexer::Type> (token, type));
  }

  t.is ((int)tokens.size (),       7,                          "7 tokens");
  t.is (tokens[0].first,           "/foo/",                    "tokens[0] == '/foo/'");
  t.is ((int) tokens[0].second,    (int) Lexer::Type::pattern, "tokens[0] == Lexer::Type::pattern");

  t.is (tokens[1].first,           "/a\\/b/",                  "tokens[1] == '/a\\/b/'");
  t.is ((int) tokens[1].second,    (int) Lexer::Type::pattern, "tokens[1] == Lexer::Type::pattern");
  t.is (tokens[2].first,           "/'/",                      "tokens[2] == '/'/'");
  t.is ((int) tokens[2].second,    (int) Lexer::Type::pattern, "tokens[2] == Lexer::Type::pattern");

  t.is (tokens[3].first,           "desc",                     "tokens[3] == 'desc'");
  t.is ((int) tokens[3].second,    (int) Lexer::Type::dom,     "tokens[3] == Lexer::Type::dom");
  t.is (tokens[4].first,           "~",                        "tokens[4] == '~'");
  t.is ((int) tokens[4].second,    (int) Lexer::Type::op,      "tokens[4] == Lexer::Type::op");
  t.is (tokens[5].first,           "pattern",                  "tokens[5] == 'pattern'");
  t.is ((int) tokens[5].second,    (int) Lexer::Type::dom,     "tokens[5] == Lexer::Type::dom");

  t.is (tokens[6].first,           "desc.cont:pattern",        "tokens[6] == 'desc.cont:pattern'");
  t.is ((int) tokens[6].second,    (int) Lexer::Type::pair,    "tokens[6] == Lexer::Type::pair");

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
