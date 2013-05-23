solution "curvedani"
  configurations { "Debug", "Release" }
  language "C++"
--  platforms({ "Native" })
  location ( "../build/" .. _ACTION)
  defines { "WIN32", "_WINDOWS" }


  project "curvedani"
    kind "StaticLib"
    location ("../build/" .. _ACTION)
    targetdir ("../lib/" .. _ACTION)
    targetextension ".lib"
    files
    {
      "../src/**.cpp",
      "../src/**.h",
      "../include/**.h",
    }
    excludes
    {
    }
    includedirs
    {
      "../include",
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
    configuration "Debug"
      defines { "_DEBUG" }
      targetname "curvedani_d"
      flags
      {
      }

    configuration "Release"
      defines { "NDEBUG" }
      targetname "curvedani"
      flags
      {
	 "OptimizeSize",
      }
