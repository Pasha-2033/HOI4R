/*
//��������� ������
float4 VS(float4 Pos : POSITION) : SV_POSITION {
	// ��������� ���������� ����� ��� ���������
	return Pos;
}
//���������� ������
float4 PS(float4 Pos : SV_POSITION) : SV_Target {
	// ���������� ������ ����, ������������ (����� == 1, �����-����� �� �������).	
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
	float xshift = 250.0f;
	float yshift = 250.0f;//282.5f; ��� ������������
	float K = 50.0f;
	float dist = sqrt((Pos.x - xshift) * (Pos.x - xshift) + (Pos.y - yshift) * (Pos.y - yshift));
	dist = (dist % K) / K;
	return float4(dist, 0.0f, 0.0f, 1.0f);
}*/


//--------------------------------------------------------------------------------------
// ���������� ����������� �������
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0) // b0 - ������ ������
{
	matrix World;
	matrix View;
	matrix Projection;
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT	// ������ �������� ������ ���������� �������
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

//--------------------------------------------------------------------------------------
// ��������� ������
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION, float4 Color : COLOR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	// ������������� ������� ������� ��� ������ ��������� �� �������
	output.Pos = mul(Pos, World); // ������� � ������������ ����
	output.Pos = mul(output.Pos, View); // ����� � ������������ ����
	output.Pos = mul(output.Pos, Projection); // � ������������ ������������
	output.Color = Color;
	return output;
}


//--------------------------------------------------------------------------------------
// ���������� ������
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
	float xshift = 250.0f;
	float yshift = 250.0f;//282.5f; ��� ������������
	float K = 50.0f;
	float dist = sqrt((input.Pos.x - xshift) * (input.Pos.x - xshift) + (input.Pos.y - yshift) * (input.Pos.y - yshift));
	dist = (dist % K) / K;
	return input.Color * dist;
	//return input.Color;
}