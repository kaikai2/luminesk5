project "glportdemo"
    kind "WindowedApp"
    location ("../build/" .. _ACTION)
    targetdir ("../bin")
    targetextension ".exe"
    defines { "_WINDOWS" }
    links { "glport", "curvedani" }
    files
    {
      "../src/demo/**.cpp",
      "../src/demo/**.h",
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
      "../lib/" .. _ACTION,
      "../../curvedani/lib/" .. _ACTION
    }
    flags
    {
      "NoPCH",
      "StaticRuntime",
      "WinMain"
    }
    configuration "Debug"
      defines { "_DEBUG" }
      targetname "glportdemo_d"
      links
      {
        "glport_d",
        "curvedani_d",
	"opengl32",
	"winmm",
	"SDL",
	"SDL_image",
	"SDL_ttf",
	"SDLmain"
      }
      flags
      {
      	"Symbols", 
      }
    configuration "Release"
      defines { "NDEBUG" }
      targetname "glportdemo"
      links
      {
        "glport",
        "curvedani",
	"opengl32",
	"winmm",
	"SDL",
	"SDL_image",
	"SDL_ttf",
	"SDLmain"
      }
      flags
      {
	 "OptimizeSize",
      }
