/*!
 * \brief   The file contains class implementation
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2011-11-18
            \endverbatim
 * Copyright: (c) Alexander Egorov 2011
 */

#include "HLINQTest.h"
using namespace std;

void HLINQTest::SetUp()
{
    cout_stream_buffer_ = cout.rdbuf(oss_.rdbuf());
}

void HLINQTest::TearDown()
{
    cout.rdbuf(cout_stream_buffer_);
    psr_->free(psr_);
    psr_ = NULL;
    tstream_->free(tstream_);
    tstream_ = NULL;
    lxr_->free(lxr_);
    lxr_ = NULL;
    input_->close(input_);
    input_ = NULL;
}

void HLINQTest::Run(const char* q, BOOL dontRunActions)
{
    const char* utf8 = FromAnsiToUtf8(q, pool_);
    input_   = antlr3StringStreamNew((pANTLR3_UINT8)utf8, ANTLR3_ENC_UTF8, strlen(q), (pANTLR3_UINT8)"");
    lxr_     = HLINQLexerNew(input_);     // HLINQLexerNew is generated by ANTLR
    tstream_ = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lxr_));
    psr_     = HLINQParserNew(tstream_);  // HLINQParserNew is generated by ANTLR3
    psr_->prog(psr_, pool_, dontRunActions);
}

void HLINQTest::ValidateNoError()
{
    ASSERT_STREQ("", oss_.str().c_str());
}

void HLINQTest::ValidateError()
{
    ASSERT_TRUE(oss_.str().length() > 0);
}

TEST_F(HLINQTest, Comment) {
    Run("# Comment\nfor f in 'c:' do delete;");
    ValidateNoError();
}

TEST_F(HLINQTest, DelFile) {
    Run("for f in 'c:' do delete;");
    ValidateNoError();
}

TEST_F(HLINQTest, PrnFileName) {
    Run("for f in 'c:' do print f.name;");
    ValidateNoError();
}

TEST_F(HLINQTest, DelFileNameEq) {
    Run("for f in 'c:' where f.name = 'test' do delete;");
    ValidateNoError();
}

TEST_F(HLINQTest, CalcStrHash) {
    Run("for '123' do md5;");
    ValidateNoError();
}

TEST_F(HLINQTest, CalcStrHashRun) {
    Run("for '123' do md5;", FALSE);
    ValidateNoError();
}

TEST_F(HLINQTest, TwoQueries) {
    Run("for '123' do sha1;for '123' do md5;");
    ValidateNoError();
}

TEST_F(HLINQTest, CrackStr) {
    Run("for 'D41D8CD98F00B204E9800998ECF8427E' as s let s.min = 4 do crack md5;");
    ValidateNoError();
}

TEST_F(HLINQTest, NoQueryEnd) {
    Run("for f in 'c:' do delete");
    ValidateError();
}

TEST_F(HLINQTest, InvalidId) {
    Run("for f in 'c:' where f1.size = 0 do delete;");
    ValidateError();
}

TEST_F(HLINQTest, InvalidAttr) {
    Run("for f in 'c:' do print f.md;");
    ValidateError();
}

TEST_F(HLINQTest, PrnFileAttrCall) {
    Run("for f in 'c:' do print f.name;");
    ValidateNoError();
}

TEST_F(HLINQTest, PrnFileAttrCallTwice) {
    Run("for f in 'c:' do print f.name + f.size;");
    ValidateNoError();
}

TEST_F(HLINQTest, PrnFileAttrCallAndStrAfter) {
    Run("for f in 'c:' do print f.name + ' Kb';");
    ValidateNoError();
}

TEST_F(HLINQTest, PrnFileAttrCallAndStrBefore) {
    Run("for f in 'c:' do print 'File: ' + f.name;");
    ValidateNoError();
}

TEST_F(HLINQTest, PrnFileAttrCallAndStrBeforeAndAfter) {
    Run("for f in 'c:' do print 'File: ' + f.name + ' Kb';");
    ValidateNoError();
}

TEST_F(HLINQTest, PrnFileAttrCallStripedWithStr) {
    Run("for f in 'c:' do print 'File: ' + f.name + ' Size: ' + f.size;");
    ValidateNoError();
}

TEST_F(HLINQTest, PrnFileAttrCallStripedWithStrStrAfter) {
    Run("for f in 'c:' do print 'File: ' + f.name + ' Size: ' + f.size + ' Kb';");
    ValidateNoError();
}

TEST_F(HLINQTest, PrnFileOneStr) {
    Run("for f in 'c:' do print 'File ';");
    ValidateNoError();
}

TEST_F(HLINQTest, PrnFileTwoStr) {
    Run("for f in 'c:' do print 'File ' + 'attr called';");
    ValidateNoError();
}

TEST_F(HLINQTest, WhereSimple) {
    Run("for f in 'c:' where f.size = 0 do delete;");
    ValidateNoError();
}

TEST_F(HLINQTest, WhereTwoAnd) {
    Run("for f in 'c:' where f.size = 0 and f.name ~ '*.exe' do delete;");
    ValidateNoError();
}

TEST_F(HLINQTest, WhereTwoOr) {
    Run("for f in 'c:' where f.size = 0 or f.name ~ '*.exe' do delete;");
    ValidateNoError();
}

TEST_F(HLINQTest, WhereBraces) {
    Run("for f in 'c:' where f.size = 0 and (f.name ~ '*.exe' or f.path ~ 'c:\\\\temp\\\\*') do delete;");
    ValidateNoError();
}

TEST_F(HLINQTest, WhereBracesUnclosed) {
    Run("for f in 'c:' where f.size = 0 and (f.name ~ '*.exe' or f.path ~ 'c:\\\\temp\\\\*' do delete;");
    ValidateError();
}