function dumpTable(table, depth)
    debug("dumping")

  if (depth > 200) then
    debug("Error: Depth > 200 in dumpTable()")
    return
  end
  for k,v in pairs(table) do
    if (type(v) == "table") then
        if k ~= "__index" then
            debug(string.rep(" ", depth)..k..":")
            dumpTable(v, depth+1)
        end
    else
      debug(string.rep(" ", depth)..k..": "..v)
    end
  end

  debug("dumped")
end
