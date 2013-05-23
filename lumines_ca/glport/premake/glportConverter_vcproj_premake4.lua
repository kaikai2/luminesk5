project "glportConverter"
    kind "ConsoleApp"
    location ("../build/" .. _ACTION)
    targetdir ("../bin")
    targetextension ".exe"
    defines { "_CONSOLE" }
    links { "glport", "curvedani" }
    files
    {
      "../src/converter/**.cpp",
      "../src/converter/**.h",
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
      targetname "glportcvt_d"
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
      targetname "glportcvt"
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
