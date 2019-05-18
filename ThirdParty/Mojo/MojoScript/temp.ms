local interface Person
  firstname:string
  middlename:string?
  lastname:string
end

local user1: Person = { firstname = "Lewis", middlename = "Allan", lastname = "Reed" }
local user2: Person = { lastname = "Reed", firstname = "Lou" }