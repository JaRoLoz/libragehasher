project "libragehasher"
    kind "StaticLib"
    language "C++"
    cppdialect "C++23"

    files { "include/**.h*", "**.c*" }
    includedirs { "include", "../sha1" }
    links { "sha1" }