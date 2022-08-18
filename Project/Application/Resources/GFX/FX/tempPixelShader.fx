/*
//Вершинный шейдер
float4 VS(float4 Pos : POSITION) : SV_POSITION {
	// Оставляем координаты точки без изменений
	return Pos;
}
//Пиксельный шейдер
float4 PS(float4 Pos : SV_POSITION) : SV_Target {
	// Возвращаем желтый цвет, непрозрачный (альфа == 1, альфа-канал не включен).	
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
	float xshift = 250.0f;
	float yshift = 250.0f;//282.5f; для треугольника
	float K = 50.0f;
	float dist = sqrt((Pos.x - xshift) * (Pos.x - xshift) + (Pos.y - yshift) * (Pos.y - yshift));
	dist = (dist % K) / K;
	return float4(dist, 0.0f, 0.0f, 1.0f);
}*/


//--------------------------------------------------------------------------------------
// Переменные константных буферов
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0) // b0 - индекс буфера
{
	matrix World;
	matrix View;
	matrix Projection;
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT	// формат выходных данных вершинного шейдера
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

//--------------------------------------------------------------------------------------
// Вершинный шейдер
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION, float4 Color : COLOR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	// Трансформация позиции вершины при помощи умножения на матрицу
	output.Pos = mul(Pos, World); // сначала в пространство мира
	output.Pos = mul(output.Pos, View); // затем в пространство вида
	output.Pos = mul(output.Pos, Projection); // в проекционное пространство
	output.Color = Color;
	return output;
}


//--------------------------------------------------------------------------------------
// Пиксельный шейдер
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
	float xshift = 250.0f;
	float yshift = 250.0f;//282.5f; для треугольника
	float K = 50.0f;
	float dist = sqrt((input.Pos.x - xshift) * (input.Pos.x - xshift) + (input.Pos.y - yshift) * (input.Pos.y - yshift));
	dist = (dist % K) / K;
	return input.Color * dist;
	//return input.Color;
}