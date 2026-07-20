// Copyright 2025 solar-mist

#ifndef VLEX_GENERATOR_GENERATOR_H
#define VLEX_GENERATOR_GENERATOR_H 1

#include "generator/Symbol.h"

#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace parser
{
    struct TokenDescriptor;
    struct CommentDescriptor;
}

namespace generator
{
    struct NamingPolicy {
        enum class Convention {
            SnakeCase,
            PascalCase,
        };

        enum class Identifier {
            Lexer,
            Token,
            SourceLocation,
        };

        std::string fileNamePrefix;
        Convention fileNameConvention;

        std::string classNamePrefix;
        Convention classNameConvention;

        void fixPrefixes();

        std::string fileName(Identifier id, std::optional<std::string_view> ext = std::nullopt) const;
        std::string className(Identifier id) const;

        static Convention ConventionFromName(std::string_view name);

    private:
        static std::span<const std::string_view> GetWords(Identifier id);
        static std::string Format(std::span<const std::string_view> words, Convention convention);
    };

    class Generator
    {
    public:
        Generator(std::string namespaceName, std::vector<parser::TokenDescriptor> symbols, std::vector<parser::TokenDescriptor> keywords, std::vector<parser::TokenDescriptor> specials, std::vector<parser::CommentDescriptor> comments);

        void generate(std::filesystem::path outsource, std::filesystem::path outinc, const NamingPolicy& namingPolicy);

    private:
        std::string mNamespaceName;
        std::vector<parser::TokenDescriptor> mSymbols;
        std::vector<parser::TokenDescriptor> mKeywords;
        std::vector<parser::TokenDescriptor> mSpecials;
        std::vector<parser::CommentDescriptor> mComments;

        std::vector<std::unique_ptr<Symbol> > mSymbolNodes;

        void generateSourceLocation(std::filesystem::path outsource, std::filesystem::path outinc, const NamingPolicy& namingPolicy);
        void generateToken(std::filesystem::path outsource, std::filesystem::path outinc, const NamingPolicy& namingPolicy);
        void generateLexer(std::filesystem::path outsource, std::filesystem::path outinc, const NamingPolicy& namingPolicy);


        void generateSpecials(std::ofstream& stream);
        void generateSymbols(std::ofstream& stream);


        std::string symbolToName(std::string symbol);
    };
}

#endif // VLEX_GENERATOR_GENERATOR_H