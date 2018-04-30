#pragma once
#include <string>
#include <functional>
#include "Type.h"
#include "..\shared\Games.h"
#include "Tabs.h"
#include "ListEx.h"

// TODO: function parameter default value CVector:arg(CVector())

using namespace std;

class Struct;

class Function {
public:
    enum class Usage {
        Default,
        DefaultConstructor,
        CustomConstructor,
        BaseDestructor,
        DeletingDestructor,
        CopyConstructor,
        Operator,
        OperatorNew,
        OperatorDelete
    };

    enum CC {
        CC_CDECL,
        CC_STDCALL,
        CC_FASTCALL,
        CC_THISCALL,
        CC_UNKNOWN = 255
    };

    struct Reference {
        int mRefAddr = 0;
        int mGameVersion = -1;
        int mRefType = -1;
        int mRefObjectId = -1;
        int mRefIndexInObject = -1;
    };

    Struct *mClass = nullptr;      // ptr to class (nullptr if there's no class)

    string mName;                  // function name (without class name and scope)
    string mMangledName;           // mangled name
    string mScope;                 // scope (scope::function_name)
    string mClassName;             // class name
    string mModuleName;            // module name
    CC mCC = CC_CDECL;             // calling convention
    Type mRetType;                 // function return type

    bool mIsConst = false;         // has 'const' attribute
    bool mIsEllipsis = false;      // variable parameter count ('...')
    bool mIsOverloaded = false;    // is overloaded (one or more functions with same name in the class)
    bool mIsStatic = false;        // static function (function with __cdecl calling convention inside class)
    int mRVOParamIndex = -1;       // where's RVO parameter placed
    int mNumParamsToSkipForWrapper = 0; // count of 'special' parameters (this pointer, RVO parameter)
    string mComment;               // function comment
    string mType;                  // function declaration
    unsigned int mPriority = 1;    // priority for events (before/after)
    Usage mUsage = Usage::Default; // constructor/destructor/etc.
    int mVTableIndex = -1;         // function index in virtual table
    bool mIsVirtual = false;       // function is virtual (placed in virtual table)
    bool mIgnore = false;          // don't write this function in source files

    struct Parameter {
        string mName;
        Type mType;
    };

    Vector<Parameter> mParameters;

    struct ExeVersionInfo {
        unsigned int mAddress = 0;
        string mRefsStr;
    };

    ExeVersionInfo mVersionInfo[Games::GetMaxGameVersions()];

    string GetFullName() const; // combine name + scope

    void WriteDefinition(ofstream &stream, tabs t, Games::IDs game);
    void WriteDeclaration(ofstream &stream, tabs t, Games::IDs game);
    void WriteMeta(ofstream &stream, tabs t, Games::IDs game);

    string NameForWrapper(Games::IDs game, bool definition);
    string MetaDesc();
    string AddrOfMacro(bool global);
    string Addresses(Games::IDs game);
    bool IsConstructor();
    bool IsDestructor();
};
