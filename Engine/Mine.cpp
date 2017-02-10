#include "MainWindow.h"
#include "Mine.h"
#include "Ship.h"
#include "Surface.h"

Mine::Mine(float X, const Surface& MineSurface, Sound& Explosion, AnimationFrames& Anim)
	:
	surface(MineSurface),
	explo(Anim, 2),
	explosion(Explosion),
	x(X)
{}

void Mine::HandleCollision(int dmg)
{
	hp -= dmg;
	switch (mState)
	{
	case MineState::ActiveState:
		if (hp <= 0)
		{
			damage = explDamage;
			SetState(Mine::DetonateState);
			explosion.Play();
		}
		break;
	case MineState::DetonateState:
		damage = 0;
		break;
	}
}

void Mine::Update(float Dt)
{
	switch (mState)
	{
	case MineState::ActiveState:
		y += vy * Dt;
		break;
	case MineState::DetonateState:
		explo.Advance();
		if (explo.AnimEnd())
			mState = InActiveState;
		break;
	}
}

void Mine::Draw(Graphics& gfx)
{
	switch (mState)
	{
	case ActiveState:
		if (y < gfx.ScreenHeight)
		{
			gfx.DrawSpriteKey(int(x), int(y), surface, surface.GetPixel(0, 0));
		}
		break;
	case DetonateState:
		explo.Draw(int(x), int(y), gfx);
		break;
	}
}

int Mine::GetDamageCost() const
{
	return damage;
}

void Mine::SetState(MineState State)
{
	mState = State;
}

RectF Mine::GetCollisionRect() const
{
	return RectF(x, y, width, height);
}

bool Mine::IsActive() const
{
	return mState == ActiveState;
}

void Mine::Reset()
{
	y = -50.0f;
	hp = 60;
	mState = ActiveState;
	explo.Reset();
}

//float Mine::GetX() const
//{
//	return x;
//}
//
//float Mine::GetY() const
//{
//	return y;
//}
//
//float Mine::GetWidth() const
//{
//	return width;
//}
//
//float Mine::GetHeight() const
//{
//	return height;
//}