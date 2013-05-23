solution "glport"
	configurations { "Debug", "Release" }
	language "C++"
	--  platforms({ "Native" })
	location ( "../build/" .. _ACTION)
	defines { "WIN32", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS" }

dofile  "../../curvedani/premake/curvedani_vcproj_premake4.lua"
dofile  "glport_vcproj_premake4.lua"
dofile  "glportdemo_vcproj_premake4.lua"
dofile  "glportConverter_vcproj_premake4.lua"
