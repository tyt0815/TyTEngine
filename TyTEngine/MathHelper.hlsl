float GetDistance(float3 Pos1, float3 Pos2)
{
	float3 Diff = Pos2 - Pos1;
	return sqrt(dot(Diff, Diff));
}