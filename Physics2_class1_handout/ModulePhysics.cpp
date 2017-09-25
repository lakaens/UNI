#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"



#define METERS_TO_PIXELS(x) x*1
#define PIXELS_TO_METERS(x) x/1


// TODO 1: Include Box 2 header and library

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
	gravity = b2Vec2 (0.0f, 10.0f);
	world = new b2World(gravity);
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	// TODO 2: Create a private variable for the world
	// - You need to send it a default gravity
	// - You need init the world in the constructor
	// - Remember to destroy the world after using it
	
	

	// TODO 4: Create a a big static circle as "ground"
	groundBCircle.position.Set(PIXELS_TO_METERS(SCREEN_WIDTH/2), PIXELS_TO_METERS(SCREEN_HEIGHT/2));
	groundbCircle = world->CreateBody(&groundBCircle);
	circle.m_radius = PIXELS_TO_METERS(250);
	groundbCircle->CreateFixture(&circle, 0.0f);
	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	// TODO 3: Update the simulation ("step" the world)

	world->Step(1.0f / 60.0f, 8, 3);

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {


		b2Vec2 pos(App->input->GetMouseX(), App->input->GetMouseY());

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(pos.x, pos.y);
		b2Body* body = world->CreateBody(&bodyDef);

		b2CircleShape dynamicCircle;
		dynamicCircle.m_p.Set(0, 0); //position, relative to body position
		dynamicCircle.m_radius = 30;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicCircle;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;

		body->CreateFixture(&fixtureDef);
	}
	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();
				App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
			}
			break;

			

			// You will have to add more cases to draw boxes, edges, and polygons ...
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	if (world != NULL)
		delete world;

	return true;
}
