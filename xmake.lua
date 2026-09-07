add_rules("mode.release")

if is_plat("windows") then
    local msvcRuntime = is_mode("debug") and "MTd" or "MT"
    set_runtimes(msvcRuntime)
end

-- requires
add_requires("sqlite3",{version = "3-3.53.0+0"}, {configs = {shared = false}, system = false})
add_requires("cjson",{configs = {shared = false}})
add_requires("klib",{version = "2024.06.03"}, {configs = {shared = false}})

set_warnings("all", "extra")

target("llsql")
    set_languages("c17")
    set_kind("binary")

    add_files("llsql/**.c")
    add_headerfiles("llsql/**.h")
    add_includedirs(".", {public = true})  
    set_targetdir(path.join("$(builddir)",""))             

    -- packages
    add_packages( "cjson","sqlite3","klib",
        {public = true}
    )

    -- platform defines
    if     is_plat("linux")   then
        add_defines("LLSQL_LINUX", "_GNU_SOURCE", {public = true})
        add_cxxflags("-frtti", "-fexceptions")
    elseif is_plat("windows") then
        add_defines("LLSQL_WINDOWS", {public = true})
    elseif is_plat("macosx")  then
        add_defines("LLSQL_MACOS", {public = true})
    end

    -- mode defines
    if     is_mode("debug")   then
        add_defines("LLSQL_DEBUG", {public = true})
    elseif is_mode("release") then
        add_defines("LLSQL_RELEASE", {public = true})
        if not is_plat("macosx") then
            set_policy("build.optimization.lto", true)
        end
    end
