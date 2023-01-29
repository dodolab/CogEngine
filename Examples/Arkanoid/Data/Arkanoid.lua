
----------------------------------------------------------------------------------

FILE_SOUND_HIT =  "hit.wav"
FILE_SOUND_ROUND = "round.mp3"
FILE_SOUND_GAME_OVER = "gameover.mp3"
FILE_SOUND_GAME_START = "intro.mp3"

HIT_TYPE_BORDER = 0
HIT_TYPE_PADDLE = 1
HIT_TYPE_BLOCK = 2

ARK_BRICK_NONE = 0
ARK_BRICK_1 = 1
ARK_BRICK_2 = 2
ARK_BRICK_3 = 3
ARK_BRICK_4 = 4
ARK_BRICK_5 = 5
ARK_BRICK_6 = 6

ARK_BRICK_INDESTRUCTIBLE = ARK_BRICK_1

----------------------------------------------------------------------------------


ArkanoidSoundComponent = Behavior:Extend("ArkanoidSoundComponent")

function ArkanoidSoundComponent:OnInit()
  self:SubscribeForMessages("ARK_EVENT_GAME_STARTED");
  self:SubscribeForMessages("ARK_EVENT_ROUND_STARTED");
  self:SubscribeForMessages("ARK_EVENT_OBJECT_HIT");
  self:SubscribeForMessages("ARK_EVENT_GAME_OVER");
  self:SubscribeForMessages("ARK_EVENT_LEVEL_COMPLETED");
  self:SubscribeForMessages("ARK_EVENT_LEVEL_STARTED");
  self:SubscribeForMessages("ARK_EVENT_GAME_COMPLETED");
end

function ArkanoidSoundComponent:OnMessage(msg)
  if(msg:GetAction() == StrId("ARK_EVENT_ROUND_STARTED")) then CogPlaySound(FILE_SOUND_ROUND)
  elseif(msg:GetAction() == StrId("ARK_EVENT_OBJECT_HIT")) then CogPlaySound(FILE_SOUND_HIT)
  elseif(msg:GetAction() == StrId("ARK_EVENT_GAME_OVER")) then CogPlaySound(FILE_SOUND_GAME_OVER)
  elseif(msg:GetAction() == StrId("ARK_EVENT_LEVEL_COMPLETED")) then CogPlaySound(FILE_SOUND_GAME_OVER)
  elseif(msg:GetAction() == StrId("ARK_EVENT_LEVEL_STARTED")) then CogPlaySound(FILE_SOUND_ROUND)
  elseif(msg:GetAction() == StrId("ARK_EVENT_GAME_STARTED")) then CogPlaySound(FILE_SOUND_GAME_START)
  elseif(msg:GetAction() == StrId("ARK_EVENT_GAME_COMPLETED")) then  CogPlaySound(FILE_SOUND_GAME_OVER) end
end

----------------------------------------------------------------------------------

ArkanoidIntroComponent = Behavior:Extend("ArkanoidIntroComponent")

function ArkanoidIntroComponent:OnInit()
  self.introShowDelay = 5000
  self.introShowTime = 0
  self.model = self.owner:GetSceneRoot():GetAttr_ARKANOID_MODEL()
  self:SendMessage("ARK_EVENT_GAME_STARTED")
end

function ArkanoidIntroComponent:Update(delta, absolute)
  self.introShowTime = self.introShowTime + delta
  
  if(self.introShowTime > self.introShowDelay) then
    self.model.currentLevel = 1
	CogGetComponent_ArkanoidFactory():ResetGame()
  end
end

----------------------------------------------------------------------------------

PaddleController = Behavior:Extend("PaddleController")


function PaddleController:OnInit()
  self.ball = self.owner:GetScene():FindNodeByTag("ball")
  self.model = self.owner:GetSceneRoot():GetAttr_ARKANOID_MODEL()
  self.leftDirection = false
  self.paddleLastPos = 0
  self.left = false
end

function PaddleController:Move(left, delta)
  local trans = self.owner:GetTransform()
  local model = self.model
  
  if(left) then
    trans:GetLocalPos().x = trans.localPos.x - model.paddleSpeed * delta
  else
    trans:GetLocalPos().x = trans.localPos.x + model.paddleSpeed * delta
  end

  trans:GetLocalPos().x = math.max(math.min(trans.localPos.x, 20), 1)

  if(not model.ballReleased) then
    self.ball:GetTransform():GetLocalPos().x = trans.localPos.x + model.ballOffset
  end
end

function PaddleController:ReleaseBall()
  if(not self.model.ballReleased) then
    self.model.ballReleased = true
	local movement = self.ball:GetAttr_DYNAMICS()
	local newSpeedX
	
	if(self.leftDirection) then newSpeedX = self.model.ballSpeed else newSpeedX = self.model.ballSpeed * -1 end
   
    movement:SetVelocity(ofVec2f(newSpeedX, -self.model.ballSpeed))   
	
  end
end



PaddleInputController = PaddleController:Extend("PaddleInputController")

function PaddleInputController:Update(delta, absolute)
  if(CogIsKeyPressed(OF_KEY_LEFT)) then
    self:Move(true, delta)
  end
  
  if(CogIsKeyPressed(OF_KEY_RIGHT)) then
    self:Move(false, delta)
  end
  
  if(CogIsKeyPressed(OF_KEY_ALT)) then
    self:ReleaseBall()
  end
end


----------------------------------------------------------------------------------

ArkanoidLifeComponent = Behavior:Extend("ArkanoidLifeComponent")

function ArkanoidLifeComponent:OnInit()
  self:SubscribeForMessages("ARK_EVENT_LIFE_LOST")
  self.model = self.owner:GetSceneRoot():GetAttr_ARKANOID_MODEL()
end

function ArkanoidLifeComponent:OnMessage(msg)
  if(msg:GetAction() == StrId("ARK_EVENT_LIFE_LOST")) then
    local lifeName = string.format("life_%d", self.model.currentLives + 1)
	local life = self.owner:GetScene():FindNodeByTag(lifeName)
	life:RemoveFromParent()
  end
end

----------------------------------------------------------------------------------

ArkanoidStatusComponent = Behavior:Extend("ArkanoidStatusComponent")

function ArkanoidStatusComponent:OnInit()
  self.statusShowDelay = 3000
  self.statusShowTime = 0
  self.isShowing = 0
  self.model = self.owner:GetSceneRoot():GetAttr_ARKANOID_MODEL()
  
  self:SubscribeForMessages("ARK_EVENT_LEVEL_STARTED")
  self:SubscribeForMessages("ARK_EVENT_ROUND_STARTED")
  self:SubscribeForMessages("ARK_EVENT_GAME_OVER")
  self:SubscribeForMessages("ARK_EVENT_LEVEL_COMPLETED")
  self:SubscribeForMessages("ARK_EVENT_GAME_COMPLETED")
end

function ArkanoidStatusComponent:ShowText(text)
  local mesh = self.owner:GetText()
  mesh:SetIsVisible(true)
  mesh:SetText(text)
  self.isShowing = true
  self.statusShowTime = 0
end

function ArkanoidStatusComponent:OnMessage(msg)
  if(msg:GetAction() == StrId("ARK_EVENT_LEVEL_STARTED")) then
    self:ShowText(string.format("LEVEL %d", self.model.currentLevel))
  elseif(msg:GetAction() == StrId("ARK_EVENT_ROUND_STARTED")) then
    self:ShowText(string.format("ROUND %d", self.model.currentRound))
  elseif(msg:GetAction() == StrId("ARK_EVENT_GAME_OVER")) then
    self:ShowText("GAME OVER")
  elseif(msg:GetAction() == StrId("ARK_EVENT_LEVEL_COMPLETED")) then
    self:ShowText("LEVEL COMPLETED")
  elseif(msg:GetAction() == StrId("ARK_EVENT_GAME_COMPLETED")) then
    self:ShowText("!!!YOU WIN THE GAME!!!")
  end
end


function ArkanoidStatusComponent:Update(delta, absolute)
  if(self.isShowing) then
    self.statusShowTime = self.statusShowTime + delta
	if(self.statusShowTime >self.statusShowDelay) then
	  self.statusShowTime = 0
	  self.owner:GetText():SetIsVisible(false)
	  self.isShowing = false
	end
  end
end



----------------------------------------------------------------------------------

BrickCollisionComponent = Behavior:Extend("BrickCollisionComponent")

function BrickCollisionComponent:OnInit()
  self:SubscribeForMessages("ARK_EVENT_OBJECT_HIT")
  self.model = self.owner:GetSceneRoot():GetAttr_ARKANOID_MODEL()
  self.bricks = self.owner:GetScene():FindNodeByTag("bricks")
end


function BrickCollisionComponent:OnMessage(msg)
  if(msg:GetAction() == StrId("ARK_EVENT_OBJECT_HIT")) then
    local hitInfo = msg:GetData_HITINFO()
	if(hitInfo.hitType == HIT_TYPE_BLOCK) then
	  self:ResolveBrickHit(hitInfo)
	end
  end
end

function BrickCollisionComponent:ResolveBrickHit(hitInfo)
  local mesh = self.bricks:GetMultiSprite()
  local sprite = mesh:GetSprite(hitInfo.hitIndex)
  local brick = self.model:GetBrick(sprite)
  
  if(brick.type ~= ARK_BRICK_INDESTRUCTIBLE) then
	self.model.remainingBricks = self.model.remainingBricks - 1
	local position = brick.position
	self.model:RemoveBrick(position)
	mesh:RemoveSprite(sprite)
	
	if(self.model.remainingBricks == 0) then
      self:SendMessage(StrId("ARK_COMMAND_FINISH_LEVEL"))	
	end
	
  end
end

----------------------------------------------------------------------------------

GameComponent = Behavior:Extend("GameComponent")

function GameComponent:OnInit()
  self:SubscribeForMessages(StrId("ARK_EVENT_BALL_OUTSIDE_AREA"))
  self:SubscribeForMessages(StrId("ARK_COMMAND_FINISH_LEVEL"))
  
  self.resetPending = false
  self.resetTime = 0
  self.resetDuration = 3000
  
  self.model = self.owner:GetSceneRoot():GetAttr_ARKANOID_MODEL()
  self.ball = self.owner:GetScene():FindNodeByTag("ball");
  self.paddle = self.owner:GetScene():FindNodeByTag("paddle")
  
  if(self.model.currentRound == 0) then self:ResolveNextRound() end
end


function GameComponent:OnMessage(msg)
  if(msg:GetAction() == StrId("ARK_EVENT_BALL_OUTSIDE_AREA")) then
    self:ResolveLifeLost()
  elseif(msg:GetAction() == StrId("ARK_COMMAND_FINISH_LEVEL")) then
    self:ResolveLevelCompleted()
  end
end

function GameComponent:ResolveLevelCompleted()
  self.resetPending = true
  if(self.model.currentLevel == self.model.maxLevels) then
    self.model.currentLevel = 0
	self:SendMessage(StrId("ARK_EVENT_GAME_COMPLETED"))
  else
    self.model.currentLevel = self.model.currentLevel + 1
	self.model.ballSpeed = self.model.ballSpeed * self.model.ballSpeedMultiplier
	self:SendMessage(StrId("ARK_EVENT_LEVEL_COMPLETED"))
  end
  self.ball:Remove()
end


function GameComponent:ResolveLifeLost()
  self.model.currentLives = self.model.currentLives - 1
  self:SendMessage(StrId("ARK_EVENT_LIFE_LOST"))
  
  if(self.model.currentLives == 0) then
    self.resetPending = true
	self.model.currentLevel = 0
	self:SendMessage(StrId("ARK_EVENT_GAME_OVER"))
	self.ball:Remove()
  else 
    self:ResolveNextRound()
  end
end


function GameComponent:ResolveNextRound()
  local dynamics = self.ball:GetAttr_DYNAMICS()
  self.model.currentRound = self.model.currentRound + 1
  dynamics:SetVelocity(ofVec2f(0,0))
  self.model.ballReleased = false
  
  trBld = TransformBuilder()
  trBld:LocalPosition(ofVec2f(self.paddle:GetTransform().localPos.x + self.model.ballOffset, 22.4))
  :BuildAndReset(self.ball)
  
  if(self.model.currentRound == 1) then
    self:SendMessage("ARK_EVENT_LEVEL_STARTED")
  else 
    self:SendMessage("ARK_EVENT_ROUND_STARTED")
  end
end

function GameComponent:Update(delta, absolute)
  if(self.resetPending) then
    if(self.resetTime >= self.resetDuration) then
	  self.resetTime = 0
	  CogGetComponent_ArkanoidFactory():ResetGame()
	else
	  self.resetTime = self.resetTime + delta
	end
  end
end


----------------------------------------------------------------------------------


BallCollisionComponent = Behavior:Extend("BallCollisionComponent")

function BallCollisionComponent:OnInit()
  self.leftPanel = self.owner:GetScene():FindNodeByTag("left_panel")
  self.topPanel = self.owner:GetScene():FindNodeByTag("top_panel")
  self.rightPanel = self.owner:GetScene():FindNodeByTag("right_panel")
  self.paddle = self.owner:GetScene():FindNodeByTag("paddle")
  self.bricks = self.owner:GetScene():FindNodeByTag("bricks")
  
  self.model = self.owner:GetSceneRoot():GetAttr_ARKANOID_MODEL()
  self.dynamics = self.owner:GetAttr_DYNAMICS()
end

function BallCollisionComponent:Update(delta, absolute)
  if(self.dynamics:GetVelocity():lengthSquared() < 0.5) then
    return
  end
  
  local hitInfo = HitInfo()
  local hit = self:CheckPanelCollision(hitInfo) or self:CheckPaddleCollision(hitInfo) or self:CheckBrickCollision(hitInfo)
  
  if(hit) then
    self:SendMessageWithData(StrId("ARK_EVENT_OBJECT_HIT"), hitInfo)
  end
  
  if(self.owner:GetTransform().localPos.y >= 26) then self:SendMessage(StrId("ARK_EVENT_BALL_OUTSIDE_AREA")) end
end

function BallCollisionComponent:CheckPanelCollision(hitInfo)
  local ballBB = self.owner:GetMesh():GetBoundingBox()
  local leftPanelBB = self.leftPanel:GetMesh():GetBoundingBox()
  local rightPanelBB = self.rightPanel:GetMesh():GetBoundingBox()
  local topPanelBB = self.topPanel:GetMesh():GetBoundingBox()
  
  if(ballBB:Intersects(topPanelBB) and self.dynamics:GetVelocity().y < 0) then
    self.dynamics:GetVelocity().y = self.dynamics:GetVelocity().y * -1
	hitInfo.hitType = HIT_TYPE_BORDER
	return true
  end
  
  if(ballBB:Intersects(leftPanelBB) and self.dynamics:GetVelocity().x < 0) then
    self.dynamics:GetVelocity().x = self.dynamics:GetVelocity().x * -1
	hitInfo.hitType = HIT_TYPE_BORDER
	return true
  end
  
  if(ballBB:Intersects(rightPanelBB) and self.dynamics:GetVelocity().x > 0) then
    self.dynamics:GetVelocity().x = self.dynamics:GetVelocity().x * -1
	hitInfo.hitType = HIT_TYPE_BORDER
	return true
  end
  return false
end


function BallCollisionComponent:CheckPaddleCollision(hitInfo)
  local paddleBB = self.paddle:GetMesh():GetBoundingBox()
  local ballBB = self.owner:GetMesh():GetBoundingBox()
  

  if(self.model.ballReleased and ballBB:Intersects(paddleBB) and self.dynamics:GetVelocity().y > 0) then
    local maxDistanceFromCenter = paddleBB:GetSize().x / 2

	local distFromCenter = ballBB:GetCenter().x - paddleBB:GetCenter().x
	local percDist = distFromCenter / maxDistanceFromCenter
	local angle = 3.14 / 4 * percDist
	local length = self.dynamics:GetVelocity():length()

	self.dynamics:GetVelocity().x = length * math.sin(angle)
	self.dynamics:GetVelocity().y = -length * math.cos(angle)
	hitInfo.hitType = HIT_TYPE_PADDLE
	return true
  end
  return false
end

function BallCollisionComponent:CheckBrickCollision(hitInfo)
  local ballBB = self.owner:GetMesh():GetBoundingBox()
  local multiMesh = self.bricks:GetMultiSprite()
  local spritesNum = multiMesh:GetSpritesNum() - 1

  local velocity = self.dynamics:GetVelocity()

  for i = 0,spritesNum do
    local spr = multiMesh:GetSprite(i)
	local bb = BoundingBox()
	spr:CalcBoundingBox(bb)

	if(bb:Intersects(ballBB)) then
	  if(bb:HorizontalIntersection(ballBB) > bb:VerticalIntersection(ballBB)) then
	    if((bb.bottomLeft.y >ballBB.topLeft.y and bb.topLeft.y < ballBB.topLeft.y and velocity.y < 0) 
		  or (bb.topLeft.y < ballBB.bottomLeft.y and bb.bottomLeft.y > ballBB.bottomLeft.y and velocity.y > 0)) then
		    velocity.y = velocity.y * -1
		  end
	  else
	    if((bb.bottomRight.x > ballBB.bottomLeft.x and bb.bottomLeft.x < ballBB.bottomLeft.x and velocity.x < 0)
		  or (bb.bottomLeft.x < ballBB.bottomRight.x and bb.bottomRight.x > ballBB.bottomRight.x and velocity.x > 0)) then
		    velocity.x = velocity.x * -1
		  end
	  end
	  
	  hitInfo.hitIndex = i
	  hitInfo.hitType = HIT_TYPE_BLOCK
	  return true
	end
	
  end  
  return false
end