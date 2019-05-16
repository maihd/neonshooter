package = "Sources"
version = "scm-1"
source = {
  url = "git://github.com/maihd/Mojo.git"
}
description = {
  summary = "An Optional Type System for Lua",
  detailed = [[
    Typed Lua is a typed superset of Lua that compiles to plain Lua.
    It provides optional type annotations, compile-time type checking, and
    class-based object oriented programming through the definition of classes,
    interfaces, and modules.
  ]],
  homepage = "https://github.com/andremm/Sources",
  license = "MIT"
}
dependencies = {
  "lua >= 5.1",
  "lpeg >= 0.12"
}
build = {
  type = "builtin",
  modules = {
    ["Sources.tlast"] = "Sources/tlast.lua",
    ["Sources.tlchecker"] = "Sources/tlchecker.lua",
    ["Sources.tlcode"] = "Sources/tlcode.lua",
    ["Sources.tlfilter"] = "Sources/tlfilter.lua",
    ["Sources.tllexer"] = "Sources/tllexer.lua",
    ["Sources.tlparser"] = "Sources/tlparser.lua",
    ["Sources.tlst"] = "Sources/tlst.lua",
    ["Sources.tltype"] = "Sources/tltype.lua",
    ["Sources.tlvisitor"] = "Sources/tlvisitor.lua",
    ["Sources.tldparser"] = "Sources/tldparser.lua"
  },
  install = {
    bin = { "tlc" },
    lua = {
      ["Sources.StdLua51.base"] = "Sources/StdLua51/base.tld",
      ["Sources.StdLua51.coroutine"] = "Sources/StdLua51/coroutine.tld",
      ["Sources.StdLua51.package"] = "Sources/StdLua51/package.tld",
      ["Sources.StdLua51.string"] = "Sources/StdLua51/string.tld",
      ["Sources.StdLua51.table"] = "Sources/StdLua51/table.tld",
      ["Sources.StdLua51.math"] = "Sources/StdLua51/math.tld",
      ["Sources.StdLua51.io"] = "Sources/StdLua51/io.tld",
      ["Sources.StdLua51.os"] = "Sources/StdLua51/os.tld",
      ["Sources.StdLua51.debug"] = "Sources/StdLua51/debug.tld",
      ["Sources.StdLua52.base"] = "Sources/StdLua52/base.tld",
      ["Sources.StdLua52.coroutine"] = "Sources/StdLua52/coroutine.tld",
      ["Sources.StdLua52.package"] = "Sources/StdLua52/package.tld",
      ["Sources.StdLua52.string"] = "Sources/StdLua52/string.tld",
      ["Sources.StdLua52.table"] = "Sources/StdLua52/table.tld",
      ["Sources.StdLua52.math"] = "Sources/StdLua52/math.tld",
      ["Sources.StdLua52.bit32"] = "Sources/StdLua52/bit32.tld",
      ["Sources.StdLua52.io"] = "Sources/StdLua52/io.tld",
      ["Sources.StdLua52.os"] = "Sources/StdLua52/os.tld",
      ["Sources.StdLua52.debug"] = "Sources/StdLua52/debug.tld",
      ["Sources.StdLua53.base"] = "Sources/StdLua53/base.tld",
      ["Sources.StdLua53.coroutine"] = "Sources/StdLua53/coroutine.tld",
      ["Sources.StdLua53.package"] = "Sources/StdLua53/package.tld",
      ["Sources.StdLua53.string"] = "Sources/StdLua53/string.tld",
      ["Sources.StdLua53.utf8"] = "Sources/StdLua53/utf8.tld",
      ["Sources.StdLua53.table"] = "Sources/StdLua53/table.tld",
      ["Sources.StdLua53.math"] = "Sources/StdLua53/math.tld",
      ["Sources.StdLua53.io"] = "Sources/StdLua53/io.tld",
      ["Sources.StdLua53.os"] = "Sources/StdLua53/os.tld",
      ["Sources.StdLua53.debug"] = "Sources/StdLua53/debug.tld"
    }
  }
}
