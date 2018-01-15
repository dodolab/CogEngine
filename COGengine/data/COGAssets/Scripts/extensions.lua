
-- overridden print function
print = function(...) 

  local msg  = ""
  local args = { n = select("#", ...); ... }
  
   for i = 1, args.n do
        msg = msg .. tostring(args[i])
    end
	
   CogLogInfo("Lua",msg)
end