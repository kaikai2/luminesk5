  project "glport"
    local glport = project()
    kind "StaticLib"
    name = "glport"
    location ("../build/" .. _ACTION)
    targetdir ("../lib/" .. _ACTION)
    targetextension ".lib"
    files
    {
      "../src/*.cpp",
      "../src/*.h",
      "../include/**.h",
    }
    excludes
    {
    }
    includedirs
    {
      "../include",
      "../../curvedani/include"
    }
    libdirs
    {
      "../lib",
    }
    flags
    {
      "NoPCH",
      "StaticRuntime",
      "WinMain"
    }
    defines { "_LIB" }
    configuration "Debug"
      defines { "_DEBUG" }
      targetname "glport_d"
      flags
      {
      	"Symbols", 
      }
    configuration "Release"
      defines { "NDEBUG" }
      targetname "glport"
      flags
      {
	 "OptimizeSize",
      }
