RWTexture2D<float4> arg_0 : register(u0, space1);

void textureDimensions_fbb15a() {
  int2 tint_tmp;
  arg_0.GetDimensions(tint_tmp.x, tint_tmp.y);
  uint2 res = tint_tmp;
}

struct tint_symbol {
  float4 value : SV_Position;
};

float4 vertex_main_inner() {
  textureDimensions_fbb15a();
  return (0.0f).xxxx;
}

tint_symbol vertex_main() {
  const float4 inner_result = vertex_main_inner();
  tint_symbol wrapper_result = (tint_symbol)0;
  wrapper_result.value = inner_result;
  return wrapper_result;
}

void fragment_main() {
  textureDimensions_fbb15a();
  return;
}

[numthreads(1, 1, 1)]
void compute_main() {
  textureDimensions_fbb15a();
  return;
}
