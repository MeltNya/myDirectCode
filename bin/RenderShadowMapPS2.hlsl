struct PS_INPUT {
	float depthL : TEXCOORD1;
};

float4 my_PS(PS_INPUT pin) : SV_TARGET
{
	float dep = pin.depthL;
	if (pin.depthL < 0) {
		dep = 1.0f;
	}
	return float4(dep, dep, dep, 1.0f);
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
}
