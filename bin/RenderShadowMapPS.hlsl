

struct PS_INPUT {
	float depthL : TEXCOORD1;
};

float4 my_PS(PS_INPUT pin) : SV_TARGET
{
	return float4(pin.depthL, pin.depthL, pin.depthL, pin.depthL);
}