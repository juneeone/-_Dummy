////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_CubeModel = 0;
	m_LightShader = 0;
	m_Light1 = 0;
	m_Light2 = 0;
	m_Light3 = 0;
	m_Light4 = 0;

	m_Position = 0;
	m_Input = 0;
	m_Timer = 0;
	
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd,HINSTANCE hinstance)
{
	bool result;
	
	m_Input = new InputClass;
	if (!m_Input) {
		return false;
	}
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}
		
	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}
	

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}
	// 2D ����� �������̽� �������� ���� ī�޶�� �⺻ �� ����� �ʱ�ȭ�մϴ�.
	D3DXMATRIX baseViewMatrix;
	m_Camera->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// ī�޶��� �ʱ� ��ġ�� �����մϴ�.
	D3DXVECTOR3 camera = D3DXVECTOR3(0.0f, 2.0f, -12.0f);
	m_Camera->SetPosition(camera);

	m_Timer = new TimerClass;
	if (!m_Timer) {
		return false;
	}
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}
	m_Position = new PositionClass;
	if (!m_Position) {
		return false;
	}
	m_Position->SetPosition(camera);

	// Create the model object.
	m_Model = new ModelClass;
	if(!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	WCHAR stone[] = L"./data/GrassPatch.jpg";	//������ Ÿ�Կ� �°Բ� ���� ���� WCHAR
	char plane[] = "./data/GrassPatch.txt";

	result = m_Model->Initialize(m_D3D->GetDevice(), stone, plane);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	
	m_Model->SetPosition(0.0f,-25.0f,-15.0f);
	

	//CUBE ��
	m_CubeModel = new ModelClass;
	if (!m_CubeModel)
	{
		return false;
	}
	WCHAR Cubetex[] = L"./data/wall.jpg";	//������ Ÿ�Կ� �°Բ� ���� ���� WCHAR
	char CUBE[] = "./data/wall.txt";
	result = m_CubeModel->Initialize(m_D3D->GetDevice(), Cubetex, CUBE);
	if (!result) 
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	//m_CubeModel->SetPosition(5.0f, 12.0f, -15.0f);

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the first light object.
	m_Light1 = new LightClass;
	if(!m_Light1)
	{
		return false;
	}

	// Initialize the first light object.
	m_Light1->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_Light1->SetPosition(-3.0f, 1.0f, 3.0f);

	// Create the second light object.
	m_Light2 = new LightClass;
	if(!m_Light2)
	{
		return false;
	}

	// Initialize the second light object.
	m_Light2->SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_Light2->SetPosition(3.0f, 1.0f, 3.0f);

	// Create the third light object.
	m_Light3 = new LightClass;
	if(!m_Light3)
	{
		return false;
	}

	// Initialize the third light object.
	m_Light3->SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_Light3->SetPosition(-3.0f, 1.0f, -3.0f);

	// Create the fourth light object.
	m_Light4 = new LightClass;
	if(!m_Light4)
	{
		return false;
	}

	// Initialize the fourth light object.
	m_Light4->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light4->SetPosition(3.0f, 1.0f, -3.0f);

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the light objects.
	if(m_Light1)
	{
		delete m_Light1;
		m_Light1 = 0;
	}

	if(m_Light2)
	{
		delete m_Light2;
		m_Light2 = 0;
	}

	if(m_Light3)
	{
		delete m_Light3;
		m_Light3 = 0;
	}

	if(m_Light4)
	{
		delete m_Light4;
		m_Light4 = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}
	if (m_Position) {
		delete m_Position;
		m_Position = 0;
	}
	if (m_Timer) {
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}


	if (m_Input) {
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}
	return;
}


bool GraphicsClass::Frame()
{
	bool result;

	result = m_Input->Frame();
	if (!result) {
		return false;
	}

	if (m_Input->IsEscapePressed() == true) {
		return false;
	}
	m_Timer->Frame();
	result = HandleInput(m_Timer->GetTime());
	if (!result) {
		return false;
	}

	// Set the position of the camera.
	//m_Camera->SetPosition(0.0f, 2.0f, -12.0f);
	//m_Camera->SetPosition(D3DXVECTOR3(0.0f, 2.0f, -12.0f));


	// Render the scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}
bool GraphicsClass::HandleInput(float frameTime) {
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_Position->SetFrameTime(frameTime);

	m_Position->MoveForward(m_Input->IsWPressed());
	m_Position->MoveLeft(m_Input->IsAPressed());
	m_Position->MoveRight(m_Input->IsDPressed());
	m_Position->MoveBackward(m_Input->IsSPressed());

	m_Position->GetPosition(pos);
	m_Position->GetRotation(rot);

	m_Camera->SetPosition(pos);
	m_Camera->SetRotation(rot);
	return true;
}

bool GraphicsClass::Render()
{
	D3DXMATRIX	viewMatrix, projectionMatrix;

	D3DXMATRIX PlaneRot, PlaneMove;				//��� ��Ʈ�������� = ������ �����ϸ� ���ÿ� ó���� �ȵǱ� ������ ���� ������ �����ϰ� �Ŀ� world=planerot*planemove�� ó��
	D3DXMATRIX worldMatrix;				//Plane�� �� ���� ��Ʈ����

	D3DXMATRIX HedgeScale, HedgeMove;
	D3DXMATRIX worldMatrix2;						//CUBE�� ����� ���� ��Ʈ����

	D3DXMATRIX HedgeScale2, HedgeMove2;
	D3DXMATRIX worldMatrix3;						//CUBE�� ����� ���� ��Ʈ����

	D3DXVECTOR4 diffuseColor[4];
	D3DXVECTOR4 lightPosition[4];

	float posx = 0;			//��ǥ ����
	float posy = 0;
	float posz = 0;
	
	float rotated_ModelX;		//ȸ������ �ֱ� ���� ����

	//rotated_ModelX = D3DXToRadian(90);		//D3DXToRadian(����. ������ �Ⱦ�)


	bool result;


	// Create the diffuse color array from the four light colors.
	diffuseColor[0] = m_Light1->GetDiffuseColor();
	diffuseColor[1] = m_Light2->GetDiffuseColor();
	diffuseColor[2] = m_Light3->GetDiffuseColor();
	diffuseColor[3] = m_Light4->GetDiffuseColor();

	// Create the light position array from the four light positions.
	lightPosition[0] = m_Light1->GetPosition();
	lightPosition[1] = m_Light2->GetPosition();
	lightPosition[2] = m_Light3->GetPosition();
	lightPosition[3] = m_Light4->GetPosition();

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(255.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetWorldMatrix(worldMatrix2);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	//Cube
	D3DXMatrixTranslation(&worldMatrix2, -3.5f, -0.5f, -0.5f);
	D3DXMatrixScaling(&worldMatrix2, 1.0f, 1.0f, 1.0f);

	//worldMatrix2 = HedgeScale * HedgeMove;
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix2, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);
	
	if (!result)
	{
		return false;
	}
	
	//Cube2
	D3DXMatrixTranslation(&worldMatrix3, -5.5f, -0.5f, -0.5f);
	D3DXMatrixScaling(&worldMatrix3, 80.0f, 10.0f, 10.0f);

	worldMatrix2 = HedgeScale * HedgeMove;
	m_CubeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix3, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(), diffuseColor, lightPosition);

	if (!result)
	{
		return false;
	}

	//Plane
	D3DXMatrixScaling(&worldMatrix, 0.05f, 0.05f, 0.05f);		//�������� ������Ʈ�� ũ�⸦ �����ϱ� ������ ��.	
	D3DXMatrixRotationX(&PlaneRot, D3DXToRadian(90.0f));		//(�ٲ� ������Ʈ�� ��Ʈ����, ȸ������ �� ����)
	D3DXMatrixTranslation(&PlaneMove, 0.0f, -20.0f, 0.0f);

	worldMatrix = PlaneRot * PlaneMove;
	//D3DXMatrixRotationX(&worldMatrix, rotated_ModelX);		//(�ٲ� ������Ʈ�� ��Ʈ����, ȸ������ �� ����)

 //Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());
	//worldMatrix = D3DXMatrixTranslation( 0.0f, -0.0f, 0.0f);
	// Render the model using the light shader and the light arrays.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
								   m_Model->GetTexture(), diffuseColor, lightPosition);
	if(!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}