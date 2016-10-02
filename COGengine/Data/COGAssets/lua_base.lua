
--[[
 -- methods for testing in lua console
 
function BehaviorPrototype() 
 return  {
  RegisterDelegate = function(self, tb)
    print("Registering delegate "..tb.classname)
  end
  }
  end

function CogRegisterBehaviorPrototype(tb)
  print("Registering behavior prototype "..tb.classname)
end

--]]

-- create behavior base object
Behavior =  {
    -- function for creating instances
    New = function(self, ...)
	    local meta = self.CreateMeta(self)
		self:Constructor(...)
		local beh = BehaviorProxy()
		beh:RegisterDelegate(meta, self.classname)
		meta.proxy = beh
		
		return meta
	end,
	-- function for defining new class
	Extend = function (self, classname, subtype)
        subtype = subtype or {}
		
		if not(classname) or classname == "" then error("When called Extend(classname), a name of the new class must be specified") end
		
		local meta = self.CreateMeta(self)
		meta.classname = classname
		--CogRegisterBehaviorPrototype(meta)   not implemented yet
		return meta
    end,
	-- create metatable
	CreateMeta = function(self, subtype)
	    local meta = { __index = subtype }
        output = setmetatable({}, {
            __index = self,
			-- Behavior() and Behavior:new() are the same
			__call = function(self, ...)
			  return self:New(...)
			end
        })
		
		return output
	end,
	Constructor = function(self, ...) end,
	OnInit = function(self) end,
	OnMessage = function(self, msg) end,
	Update = function(self, delta, absolute) end,
	SubscribeForMessages = function(self, ...) return self.proxy:SubscribeForMessages(...) end,
	SendMessage = function(self, msg) return self.proxy:SendMessage(msg) end
}