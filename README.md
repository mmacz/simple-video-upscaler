# Simple video upscaler ideas

## INPUT DATA
* Video file in low resultion

## 1st processing idea

FOR each frame pair (F₁, F₂):
    1. grayscale(F₁), grayscale(F₂)
    2. flow = optical_flow(F₁ → F₂)
    3. flow = resize(flow, target_res); flow *= upscale_factor
    4. motion_mask = compute_motion_mask(flow)
    5. sharpness_map = compute_sharpness_map(F₂)
    6. F₁' = resize(F₁)
    7. F̂₂ = remap(F₁', flow)
    8. F₂' = resize(F₂)
    9. residual = F₂' - F̂₂
    10. final = blend(F̂₂, F₂', residual, motion_mask, sharpness_map)
    11. output(final)

