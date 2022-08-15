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
	float yshift = 282.5f;
	float K = 50.0f;
	float dist = sqrt((Pos.x - xshift) * (Pos.x - xshift) + (Pos.y - yshift) * (Pos.y - yshift));
	dist = (dist % K) / K;
	return float4(dist, 0.0f, 0.0f, 1.0f);
}