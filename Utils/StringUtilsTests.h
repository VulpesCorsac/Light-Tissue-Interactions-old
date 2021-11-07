#pragma once

#include "StringUtils.h"

#include <gtest/gtest.h>

using namespace Utils_NS;

TEST(StringUtilsTests, ReplaceInplace_EmptyNormal) {
    std::string s = "";
    replace_inplace(s, 'a', 'b');
    EXPECT_EQ(s, "");
}

TEST(StringUtilsTests, ReplaceInplace_EmptyDelete) {
    std::string s = "";
    replace_inplace(s, 'a', std::nullopt);
    EXPECT_EQ(s, "");
}

TEST(StringUtilsTests, ReplaceInplace_Normal) {
    std::string s = "kwa and kwa again";
    replace_inplace(s, 'a', 'b');
    EXPECT_EQ(s, "kwb bnd kwb bgbin");
}

TEST(StringUtilsTests, ReplaceInplace_Delete) {
    std::string s = "kwa and kwa again";
    replace_inplace(s, 'a', std::nullopt);
    EXPECT_EQ(s, "kw nd kw gin");
}

TEST(StringUtilsTests, Replace_EmptyNormal) {
    EXPECT_EQ(replace("", 'a', 'b'), "");
}

TEST(StringUtilsTests, Replace_EmptyDelete) {
    EXPECT_EQ(replace("", 'a', std::nullopt), "");
}

TEST(StringUtilsTests, Replace_Normal) {
    EXPECT_EQ(replace("kwa and kwa again", 'a', 'b'), "kwb bnd kwb bgbin");
}

TEST(StringUtilsTests, Replace_Delete) {
    EXPECT_EQ(replace("kwa and kwa again", 'a', std::nullopt), "kw nd kw gin");
}

TEST(StringUtilsTests, ToLowerInplace_Empty) {
    std::string s = "";
    to_lower_inplace(s);
    EXPECT_EQ(s, "");
}

TEST(StringUtilsTests, ToLowerInplace_Lower) {
    std::string s = "abcdefgh";
    to_lower_inplace(s);
    EXPECT_EQ(s, "abcdefgh");
}

TEST(StringUtilsTests, ToLowerInplace_Upper) {
    std::string s = "ABCDEFGH";
    to_lower_inplace(s);
    EXPECT_EQ(s, "abcdefgh");
}

TEST(StringUtilsTests, ToLowerInplace_Mixed) {
    std::string s = "aBcDeFgH";
    to_lower_inplace(s);
    EXPECT_EQ(s, "abcdefgh");
}

TEST(StringUtilsTests, ToLowerInplace_Symbols) {
    std::string s = " !?%-=,.";
    to_lower_inplace(s);
    EXPECT_EQ(s, " !?%-=,.");
}

TEST(StringUtilsTests, ToUpperInplace_Empty) {
    std::string s = "";
    to_upper_inplace(s);
    EXPECT_EQ(s, "");
}

TEST(StringUtilsTests, ToUpperInplace_Lower) {
    std::string s = "abcdefgh";
    to_upper_inplace(s);
    EXPECT_EQ(s, "ABCDEFGH");
}

TEST(StringUtilsTests, ToUpperInplace_Upper) {
    std::string s = "ABCDEFGH";
    to_upper_inplace(s);
    EXPECT_EQ(s, "ABCDEFGH");
}

TEST(StringUtilsTests, ToUpperInplace_Mixed) {
    std::string s = "aBcDeFgH";
    to_upper_inplace(s);
    EXPECT_EQ(s, "ABCDEFGH");
}

TEST(StringUtilsTests, ToUpperInplace_Symbols) {
    std::string s = " !?%-=,.";
    to_upper_inplace(s);
    EXPECT_EQ(s, " !?%-=,.");
}

TEST(StringUtilsTests, ToLower_Empty) {
    EXPECT_EQ(to_lower(""), "");
}

TEST(StringUtilsTests, ToLower_Lower) {
    EXPECT_EQ(to_lower("abcdefgh"), "abcdefgh");
}

TEST(StringUtilsTests, ToLower_Upper) {
    EXPECT_EQ(to_lower("ABCDEFGH"), "abcdefgh");
}

TEST(StringUtilsTests, ToLower_Mixed) {
    EXPECT_EQ(to_lower("aBcDeFgH"), "abcdefgh");
}

TEST(StringUtilsTests, ToLower_Symbols) {
    EXPECT_EQ(to_lower(" !?%-=,."), " !?%-=,.");
}

TEST(StringUtilsTests, ToUpper_Empty) {
    EXPECT_EQ(to_upper(""), "");
}

TEST(StringUtilsTests, ToUpper_Lower) {
    EXPECT_EQ(to_upper("abcdefgh"), "ABCDEFGH");
}

TEST(StringUtilsTests, ToUpper_Upper) {
    EXPECT_EQ(to_upper("ABCDEFGH"), "ABCDEFGH");
}

TEST(StringUtilsTests, ToUpper_Mixed) {
    EXPECT_EQ(to_upper("aBcDeFgH"), "ABCDEFGH");
}

TEST(StringUtilsTests, ToUpper_Symbols) {
    EXPECT_EQ(to_upper(" !?%-=,."), " !?%-=,.");
}

TEST(StringUtilsTests, GetAllVariants_Empty) {
    std::set<std::string> result;
    result.insert("");
    EXPECT_EQ(getAllVariants(""), result);
}

TEST(StringUtilsTests, GetAllVariants_SingleWord) {
    std::set<std::string> result;
    result.insert("kwa");
    EXPECT_EQ(getAllVariants("kwa"), result);
}

TEST(StringUtilsTests, GetAllVariants_WordsWithSpaces) {
    std::set<std::string> result;
    result.insert("kwa kwa");
    EXPECT_EQ(getAllVariants("kwa kwa"), result);
}

TEST(StringUtilsTests, GetAllVariants_WordsWithDashes) {
    std::set<std::string> result;
    result.insert("kwa-kwa");
    EXPECT_EQ(getAllVariants("kwa-kwa"), result);
}

TEST(StringUtilsTests, GetAllVariants_WordsWithUnderScores) {
    std::set<std::string> result;
    result.insert("kwa_kwa");
    result.insert("kwa-kwa");
    result.insert("kwa kwa");
    result.insert("kwakwa");
    EXPECT_EQ(getAllVariants("kwa_kwa"), result);
}

TEST(StringUtilsTests, SplitChar_Empty) {
    EXPECT_TRUE(split("", ' ').empty());
}

TEST(StringUtilsTests, SplitStr_Empty) {
    EXPECT_TRUE(split("", "").empty());
}

TEST(StringUtilsTests, SplitChar_NoDelim) {
    const std::string s = "abcdefgh";
    const auto result = split(s, ' ');
    ASSERT_EQ(isize(result), 1);
    EXPECT_EQ(result[0], s);
}

TEST(StringUtilsTests, SplitStr_NoDelim) {
    const std::string s = "abcdefgh";
    const auto result = split(s, " ");
    ASSERT_EQ(isize(result), 1);
    EXPECT_EQ(result[0], s);
}

TEST(StringUtilsTests, SplitChar_DelimInTheMiddle) {
    const std::string s = "abcdefgh abcdefgh";
    const auto result = split(s, ' ');
    ASSERT_EQ(isize(result), 2);
    EXPECT_EQ(result[0], "abcdefgh");
    EXPECT_EQ(result[1], "abcdefgh");
}

TEST(StringUtilsTests, SplitStr_DelimInTheMiddle) {
    const std::string s = "abcdefgh abcdefgh";
    const auto result = split(s, " ");
    ASSERT_EQ(isize(result), 2);
    EXPECT_EQ(result[0], "abcdefgh");
    EXPECT_EQ(result[1], "abcdefgh");
}

TEST(StringUtilsTests, SplitChar_DelimAtTheBeginning) {
    const std::string s = " abcdefgh";
    const auto result = split(s, ' ');
    ASSERT_EQ(isize(result), 2);
    EXPECT_EQ(result[0], "");
    EXPECT_EQ(result[1], "abcdefgh");
}

TEST(StringUtilsTests, SplitStr_DelimAtTheBeginning) {
    const std::string s = " abcdefgh";
    const auto result = split(s, " ");
    ASSERT_EQ(isize(result), 2);
    EXPECT_EQ(result[0], "");
    EXPECT_EQ(result[1], "abcdefgh");
}

TEST(StringUtilsTests, SplitChar_DelimAtTheEnd) {
    const std::string s = "abcdefgh ";
    const auto result = split(s, ' ');
    ASSERT_EQ(isize(result), 2);
    EXPECT_EQ(result[0], "abcdefgh");
    EXPECT_EQ(result[1], "");
}

TEST(StringUtilsTests, SplitStr_DelimAtTheEnd) {
    const std::string s = "abcdefgh ";
    const auto result = split(s, " ");
    ASSERT_EQ(isize(result), 2);
    EXPECT_EQ(result[0], "abcdefgh");
    EXPECT_EQ(result[1], "");
}

TEST(StringUtilsTests, SplitStr_DelimIsStr) {
    const std::string s = "abcdefgh KWA abcdefgh";
    const auto result = split(s, "KWA");
    ASSERT_EQ(isize(result), 2);
    EXPECT_EQ(result[0], "abcdefgh ");
    EXPECT_EQ(result[1], " abcdefgh");
}

TEST(StringUtilsTests, RemoveEmptyInplace_EmptyContainer) {
    std::vector<std::string> data;
    remove_empty_inplace(data);
    EXPECT_TRUE(data.empty());
}

TEST(StringUtilsTests, RemoveEmptyInplace_EmptyStringsOnly) {
    std::vector<std::string> data;
    data.push_back("");
    data.push_back("");
    data.push_back("");
    remove_empty_inplace(data);
    EXPECT_TRUE(data.empty());
}

TEST(StringUtilsTests, RemoveEmptyInplace_EmptyStringsInTheMiddle) {
    std::vector<std::string> data;
    data.push_back("kwa");
    data.push_back("");
    data.push_back("kwa");
    remove_empty_inplace(data);
    EXPECT_EQ(isize(data), 2);
    EXPECT_EQ(data[0], "kwa");
    EXPECT_EQ(data[1], "kwa");
}

TEST(StringUtilsTests, RemoveEmptyInplace_EmptyStringsAtTheBeginning) {
    std::vector<std::string> data;
    data.push_back("");
    data.push_back("kwa");
    data.push_back("kwa");
    remove_empty_inplace(data);
    EXPECT_EQ(isize(data), 2);
    EXPECT_EQ(data[0], "kwa");
    EXPECT_EQ(data[1], "kwa");
}

TEST(StringUtilsTests, RemoveEmptyInplace_EmptyStringsAtTheEnd) {
    std::vector<std::string> data;
    data.push_back("kwa");
    data.push_back("kwa");
    data.push_back("");
    remove_empty_inplace(data);
    EXPECT_EQ(isize(data), 2);
    EXPECT_EQ(data[0], "kwa");
    EXPECT_EQ(data[1], "kwa");
}

TEST(StringUtilsTests, RemoveEmpty_EmptyContainer) {
    std::vector<std::string> data;
    EXPECT_TRUE(remove_empty({}).empty());
}

TEST(StringUtilsTests, RemoveEmpty_EmptyStringsOnly) {
    std::vector<std::string> data;
    data.push_back("");
    data.push_back("");
    data.push_back("");
    EXPECT_TRUE(remove_empty(data).empty());
}

TEST(StringUtilsTests, RemoveEmpty_EmptyStringsInTheMiddle) {
    std::vector<std::string> data;
    data.push_back("kwa");
    data.push_back("");
    data.push_back("kwa");
    const auto result = remove_empty(data);
    EXPECT_EQ(isize(result), 2);
    EXPECT_EQ(result[0], "kwa");
    EXPECT_EQ(result[1], "kwa");
}

TEST(StringUtilsTests, RemoveEmpty_EmptyStringsAtTheBeginning) {
    std::vector<std::string> data;
    data.push_back("");
    data.push_back("kwa");
    data.push_back("kwa");
    const auto result = remove_empty(data);
    EXPECT_EQ(isize(result), 2);
    EXPECT_EQ(result[0], "kwa");
    EXPECT_EQ(result[1], "kwa");
}

TEST(StringUtilsTests, RemoveEmpty_EmptyStringsAtTheEnd) {
    std::vector<std::string> data;
    data.push_back("kwa");
    data.push_back("kwa");
    data.push_back("");
    const auto result = remove_empty(data);
    EXPECT_EQ(isize(result), 2);
    EXPECT_EQ(result[0], "kwa");
    EXPECT_EQ(result[1], "kwa");
}

TEST(StringUtilsTests, Join_EmptyWithEmpty) {
    EXPECT_EQ(join({}, ""), "");
}

TEST(StringUtilsTests, Join_SingleStrWithEmpty) {
    EXPECT_EQ(join({"s"}, ""), "s");
}

TEST(StringUtilsTests, Join_EmptyWithStr) {
    EXPECT_EQ(join({""}, "S", false), "" );
    EXPECT_EQ(join({""}, "S", true ), "S");
}

TEST(StringUtilsTests, Join_SingleStrWithStr) {
    EXPECT_EQ(join({"s"}, "S", false), "s" );
    EXPECT_EQ(join({"s"}, "S", true ), "sS");
}

TEST(StringUtilsTests, Join_MultyStrWithStr) {
    EXPECT_EQ(join({"s", "w"}, "S", false), "sSw" );
    EXPECT_EQ(join({"s", "w"}, "S", true ), "sSwS");
}

TEST(StringUtilsTests, PrependInplaceStr_EmptyEmpty) {
    std::string s = "";
    prepend_inplace(s, "");
    EXPECT_EQ(s, "");
}

TEST(StringUtilsTests, PrependInplaceStr_StrEmpty) {
    std::string s = "kwa";
    prepend_inplace(s, "");
    EXPECT_EQ(s, "kwa");
}

TEST(StringUtilsTests, PrependInplaceStr_EmptyStr) {
    std::string s = "kwa";
    prepend_inplace(s, "");
    EXPECT_EQ(s, "kwa");
}

TEST(StringUtilsTests, PrependInplaceStr_StrStr) {
    std::string s = "kwa";
    prepend_inplace(s, "KWA");
    EXPECT_EQ(s, "KWAkwa");
}

TEST(StringUtilsTests, PrependStr_EmptyEmpty) {
    EXPECT_EQ(prepend("", ""), "");
}

TEST(StringUtilsTests, PrependStr_StrEmpty) {
    EXPECT_EQ(prepend("kwa", ""), "kwa");
}

TEST(StringUtilsTests, PrependStr_EmptyStr) {
    EXPECT_EQ(prepend("", "kwa"), "kwa");
}

TEST(StringUtilsTests, PrependStr_StrStr) {
    EXPECT_EQ(prepend("kwa", "KWA"), "KWAkwa");
}
