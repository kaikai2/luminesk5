-- target 是预定义变量,表示编译环境的名字
project.path = "../build/" .. target
project.name = "curvedani"
project.libdir = "../lib/" .. target

package.path = "../build/" .. target
package.name = "curvedani"
package.language = "c++"
package.kind = "lib"

package.config["Debug"].bindir   = "../../lib"
package.config["Debug"].objdir = "./Debug"
package.config["Debug"].target = "curvedani_d"
package.config["Release"].bindir   = "../../lib"
package.config["Release"].objdir = "./Release"
package.config["Release"].target = "curvedani"

if (target == "vs2005") then
	package.defines = { "_CRT_SECURE_NO_WARNINGS", "_CRT_SECURE_NO_DEPRECATE" }
end
if (windows) then
	package.config["Debug"].defines = { "WIN32", "_DEBUG", "_LIB" }
	package.config["Release"].defines = { "WIN32", "NDEBUG", "_LIB" }
else
	package.config["Debug"].defines = { "_DEBUG", "_LIB" }
	package.config["Release"].defines = { "NDEBUG", "_LIB" }
end

package.config["Release"].buildflags = {"optimize-speed", "no-symbols" }
package.buildflags = {"extra-warnings", "static-runtime", "no-exceptions", "no-rtti" }

package.includepaths = { "../../include" }
package.libpaths = { "../../lib" }

package.files = { matchrecursive("../../include/*.h", "../../src/*.cpp", "../../src/*.h") }
