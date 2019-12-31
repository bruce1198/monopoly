
#pragma once

#include "../util.h"

using namespace std;
using namespace glm;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

struct A {
	vec3 pos;
	GLfloat yaw;
	GLfloat pitch;
}; 
struct V {
	vec3 pos;
	GLfloat yaw;
	GLfloat pitch;
};

// Default camera values
const GLfloat YAW = 0.0;
const GLfloat PITCH = -30.0;
const GLfloat SPEED = 3.0;
const GLfloat SENSITIVTY = 0.25;
const GLfloat ZOOM = 45.0;
const vec3 TOP_VIEW = vec3(0.0f, 15.0f, 0.0f);
const GLfloat TOP_YAW = 0.0f;
const GLfloat TOP_PITCH = -88.5f;
const vec3 SIDE_VIEW = vec3(-10.0f, 5.0f, 0.0f);
const GLfloat SIDE_YAW = 0.0f;
const GLfloat SIDE_PITCH = -30.0f;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
	// Camera Attributes
	vec3 Position;
	vec3 Front;
	vec3 Up;
	vec3 Right;
	vec3 WorldUp;
	// Eular Angles
	GLfloat Yaw;
	GLfloat Pitch;
	// Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;
	bool direction[6];

	// Constructor with vectors
	Camera(vec3 position=vec3(-10.0f, 5.0f, 0.0f), vec3 up=vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH): MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
		init();
	}
	// Constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->Position = vec3(posX, posY, posZ);
		this->WorldUp = vec3(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
		init();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	mat4 getViewMatrix() {
		return lookAt(this->Position, this->Position + this->Front, this->Up);
	}
	float getZoom() {
		return radians(Zoom);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void keyboard(Camera_Movement direction, GLfloat deltaTime) {
		GLfloat velocity = this->MovementSpeed * deltaTime;
		if (direction == FORWARD)
			this->Position += this->Front * velocity;
		if (direction == BACKWARD)
			this->Position -= this->Front * velocity;
		if (direction == LEFT)
			this->Position -= this->Right * velocity;
		if (direction == RIGHT)
			this->Position += this->Right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void mouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true) {
		xoffset *= this->MouseSensitivity;
		yoffset *= this->MouseSensitivity;

		this->Yaw += xoffset;
		this->Pitch -= yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (this->Pitch > 89.0)
				this->Pitch = 89.0;
			if (this->Pitch < -89.0)
				this->Pitch = -89.0;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		this->updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void mouseScroll(GLfloat yoffset) {
		if (this->Zoom >= 1.0 && this->Zoom <= 45.0)
			this->Zoom -= yoffset;
		if (this->Zoom <= 1.0)
			this->Zoom = 1.0;
		if (this->Zoom >= 45.0)
			this->Zoom = 45.0;
	}
	void init() {
		gotopview = false;
		gosideview = false;
		goplayerview = false;
		currentview = 0;
		pleasechoose = false;
		for (bool& d : direction) {
			d = false;
		}
		stepleft = 0;
	}
	void goTopView() {
		if (valid()) {
			currentview = 0;
			Zoom = 45.0;
			gotopview = true;
			gotopviewcnt = 0;
			a.pos = (TOP_VIEW - Position) / vec3(1250);
			v.pos = vec3(50.0)*a.pos;
			a.yaw = (TOP_YAW - Yaw) / 1250;
			v.yaw = 50.0f*a.yaw;
			a.pitch = (TOP_PITCH - Pitch) / 1250;
			v.pitch = 50.0f * a.pitch;
		}
	}
	void goSideView() {
		if (valid()) {
			currentview = 1;
			Zoom = 45.0f;
			gosideview = true;
			gosideviewcnt = 0;
			a.pos = (SIDE_VIEW - Position) / vec3(1250.0f);
			v.pos = vec3(50.0)*a.pos;
			a.yaw = (SIDE_YAW - Yaw) / 1250;
			v.yaw = 50.0f * a.yaw;
			a.pitch = (SIDE_PITCH - Pitch) / 1250;
			v.pitch = 50.0f * a.pitch;
		}
	}
	void goPlayerView(int index) {
		if (valid()) {
			int idx = index % 40;
			if (player_yaw == 360.0f && idx==0)
				Yaw = 0;
			currentview = 2;
			Zoom = 60.0;
			//get view
			switch (idx) {
			case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:
				player_view = vec3(4, 1.0, -4+idx*8.0f/11);
				player_pitch = -32.75f;
				player_yaw = 89.75f;
				break;
			case 10:case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:
				player_view = vec3(4 - (idx-10) * 8.0f / 11, 1.0, 4);
				player_pitch = -32.75f;
				player_yaw = 180.0f;
				break;
			case 20:case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:case 29:
				player_view = vec3(-4, 1.0, 4 - (idx - 20) * 8.0f / 11);
				player_pitch = -32.75f;
				player_yaw = 269.75f;
				break;
			case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:
				player_view = vec3(-4 + (idx - 30) * 8.0f / 11, 1.0, -4);
				player_pitch = -32.75f;
				player_yaw = 360.0f;
				break;
			default:
				break;
			}
			goplayerview = true;
			goplayerviewcnt = 0;
			a.pos = (player_view - Position) / vec3(1250.0);
			v.pos = vec3(50.0)*a.pos;
			a.yaw = (player_yaw - Yaw) / 1250;
			v.yaw = 50.0f * a.yaw;
			a.pitch = (player_pitch - Pitch) / 1250;
			v.pitch = 50.0f * a.pitch;
		}
	}
	void movePlayerView(int from, int step) {
		currentindex = from;
		stepleft = step;
	}
	bool update() {
		/*cout << "(" << Position.x << ", " << Position.y << ", " << Position.z << ")" << endl;
		cout << "Yaw: " << Yaw << endl;
		cout << "Pitch: " << Pitch << endl;*/
		bool r = false;
		if(currentview==2 && stepleft>0 && !goplayerview) {
			currentindex++;
			stepleft--;
			goPlayerView(currentindex);
		}
		else if (currentview == 2 && stepleft == 0 && !goplayerview) {
			pleasechoose = true;
		}
		if (pleasechoose) {
			r = true;
			pleasechoose = false;
		}
			
		if (gotopview) {
			if (gotopviewcnt < 50) {
				Position += v.pos;
				v.pos -= a.pos;
				Yaw += v.yaw;
				v.yaw -= a.yaw;
				Pitch += v.pitch;
				v.pitch -= a.pitch;
				gotopviewcnt++;
				if (v.pos.x >= 0 && Position.x >= TOP_VIEW.x || v.pos.x <= 0 && Position.x <= TOP_VIEW.x) {
					Position.x = TOP_VIEW.x;
				}
				if (v.pos.y >= 0 && Position.y >= TOP_VIEW.y || v.pos.y <= 0 && Position.y <= TOP_VIEW.y) {
					Position.y = TOP_VIEW.y;
				}
				if (v.pos.z >= 0 && Position.z >= TOP_VIEW.z || v.pos.z <= 0 && Position.z <= TOP_VIEW.z) {
					Position.z = TOP_VIEW.z;
				}
				if (v.yaw>=0 && Yaw>=TOP_YAW || v.yaw<=0 && Yaw<= TOP_YAW) {
					Yaw = TOP_YAW;
				}
				if (v.pitch >= 0 && Pitch >= TOP_PITCH || v.pitch <= 0 && Pitch <= TOP_PITCH) {
					Pitch = TOP_PITCH;
				}
			}
			else {
				gotopview = false;
			}
		}
		else if (gosideview) {
			if (gosideviewcnt < 50) {
				Position += v.pos;
				v.pos -= a.pos;
				Yaw += v.yaw;
				v.yaw -= a.yaw;
				Pitch += v.pitch;
				v.pitch -= a.pitch;
				gosideviewcnt++;
				if (v.pos.x >= 0 && Position.x >= SIDE_VIEW.x || v.pos.x <= 0 && Position.x <= SIDE_VIEW.x) {
					Position.x = SIDE_VIEW.x;
				}
				if (v.pos.y >= 0 && Position.y >= SIDE_VIEW.y || v.pos.y <= 0 && Position.y <= SIDE_VIEW.y) {
					Position.y = SIDE_VIEW.y;
				}
				if (v.pos.z >= 0 && Position.z >= SIDE_VIEW.z || v.pos.z <= 0 && Position.z <= SIDE_VIEW.z) {
					Position.z = SIDE_VIEW.z;
				}
				if (v.yaw >= 0 && Yaw >= SIDE_YAW || v.yaw <= 0 && Yaw <= SIDE_YAW) {
					Yaw = SIDE_YAW;
				}
				if (v.pitch >= 0 && Pitch >= SIDE_PITCH || v.pitch <= 0 && Pitch <= SIDE_PITCH) {
					Pitch = SIDE_PITCH;
				}
			}
			else {
				gosideview = false;
			}
		}
		else if (goplayerview) {
			if (goplayerviewcnt < 50) {
				Position += v.pos;
				v.pos -= a.pos;
				Yaw += v.yaw;
				v.yaw -= a.yaw;
				Pitch += v.pitch;
				v.pitch -= a.pitch;
				goplayerviewcnt++;
				if (v.pos.x >= 0 && Position.x >= player_view.x || v.pos.x <= 0 && Position.x <= player_view.x) {
					Position.x = player_view.x;
				}
				if (v.pos.y >= 0 && Position.y >= player_view.y || v.pos.y <= 0 && Position.y <= player_view.y) {
					Position.y = player_view.y;
				}
				if (v.pos.z >= 0 && Position.z >= player_view.z || v.pos.z <= 0 && Position.z <= player_view.z) {
					Position.z = player_view.z;
				}
				if (v.yaw >= 0 && Yaw >= player_yaw || v.yaw <= 0 && Yaw <= player_yaw) {
					Yaw = player_yaw;
				}
				if (v.pitch >= 0 && Pitch >= player_pitch || v.pitch <= 0 && Pitch <= player_pitch) {
					Pitch = player_pitch;
				}
			}
			else {
				goplayerview = false;
			}
		}
		else {
			GLfloat move_rate = 0.05f;
			if (direction[0])
				Position += move_rate * MovementSpeed * Front;
			if (direction[1])
				Position -= move_rate * MovementSpeed * Front;
			if (direction[2])
				Position -= move_rate * MovementSpeed * Right;
			if (direction[3])
				Position += move_rate * MovementSpeed * Right;
			if (direction[4])
				Position += move_rate * MovementSpeed * Up;
			if (direction[5])
				Position -= move_rate * MovementSpeed * Up;
		}
		this->updateCameraVectors();
		return r;
	}
	int getCurrentView() {
		return currentview;
	}

private:
	int currentview;
	bool gotopview;
	int gotopviewcnt;
	bool gosideview;
	int gosideviewcnt;
	bool goplayerview;
	int goplayerviewcnt;
	//player view
	vec3 player_view;
	GLfloat player_yaw;
	GLfloat player_pitch;
	int stepleft;
	int currentindex;
	bool pleasechoose;
	//
	A a;
	V v;
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors() {
		vec3 front;
		front.x = cos(radians(this->Yaw)) * cos(radians(this->Pitch));
		front.y = sin(radians(this->Pitch));
		front.z = sin(radians(this->Yaw)) * cos(radians(this->Pitch));
		this->Front = normalize(front);
		this->Right = normalize(cross(this->Front, this->WorldUp));
		this->Up = normalize(cross(this->Right, this->Front));
	}
	bool valid() {
		return !gotopview && !gosideview && !goplayerview;
	}
};