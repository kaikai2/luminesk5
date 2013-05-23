solution "Lumines"
	configurations { "Debug", "Release" }
	language "C++"
	--  platforms({ "Native" })
	location ( "../build/" .. _ACTION)
	defines { "WIN32", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS" }

dofile  "../../lumines/premake/lumines_project_premake4.lua"
dofile  "../../curvedani/premake/curvedani_vcproj_premake4.lua"
dofile  "../../glport/premake/glport_vcproj_premake4.lua"