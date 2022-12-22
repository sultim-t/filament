/*
 * Copyright (C) 2019 The Android Open Source Project
 *
* Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TNT_FILAMENT_BACKEND_VULKANUTILITY_H
#define TNT_FILAMENT_BACKEND_VULKANUTILITY_H

#include <backend/DriverEnums.h>

#include <utils/FixedCapacityVector.h>

#include <bluevk/BlueVK.h>

namespace filament::backend {

struct VulkanLayoutTransition {
    VkImage image;
    VkImageLayout oldLayout;
    VkImageLayout newLayout;
    VkImageSubresourceRange subresources;
    VkPipelineStageFlags srcStage;
    VkAccessFlags srcAccessMask;
    VkPipelineStageFlags dstStage;
    VkAccessFlags dstAccessMask;
};

void createSemaphore(VkDevice device, VkSemaphore* semaphore);
VkFormat getVkFormat(ElementType type, bool normalized, bool integer);
VkFormat getVkFormat(TextureFormat format);
VkFormat getVkFormat(PixelDataFormat format, PixelDataType type);
VkFormat getVkFormatLinear(VkFormat format);
uint32_t getBytesPerPixel(TextureFormat format);
VkCompareOp getCompareOp(SamplerCompareFunc func);
VkBlendFactor getBlendFactor(BlendFunction mode);
VkCullModeFlags getCullMode(CullingMode mode);
VkFrontFace getFrontFace(bool inverseFrontFaces);
PixelDataType getComponentType(VkFormat format);
uint32_t getComponentCount(VkFormat format);
VkComponentMapping getSwizzleMap(TextureSwizzle swizzle[4]);
VkImageViewType getImageViewType(SamplerType target);
VkImageLayout getDefaultImageLayout(TextureUsage usage);
VkShaderStageFlags getShaderStageFlags(ShaderStageFlags stageFlags);

void transitionImageLayout(VkCommandBuffer cmdbuffer, VulkanLayoutTransition transition);

// Helper function for populating barrier fields based on the desired image layout.
// This logic is specific to blitting.
VulkanLayoutTransition blitterTransitionHelper(VulkanLayoutTransition transition);

// Helper function for populating barrier fields based on the desired image layout.
// This logic is specific to texturing.
VulkanLayoutTransition textureTransitionHelper(VulkanLayoutTransition transition);

bool equivalent(const VkRect2D& a, const VkRect2D& b);
bool equivalent(const VkExtent2D& a, const VkExtent2D& b);
bool isDepthFormat(VkFormat format);
uint8_t reduceSampleCount(uint8_t sampleCount, VkSampleCountFlags mask);

template <typename OutType>
utils::FixedCapacityVector<OutType> enumerate(VkResult (*func)(uint32_t*, OutType*)) {
    uint32_t size = 0;
    VkResult result = func(&size, nullptr);
    ASSERT_POSTCONDITION(result == VK_SUCCESS && size > 0, "enumerate-0 size error");
    utils::FixedCapacityVector<OutType> ret(size);
    result = func(&size, ret.data());
    ASSERT_POSTCONDITION(result == VK_SUCCESS, "enumerate-0 error");
    return move(ret);
}

template <typename InType, typename OutType>
utils::FixedCapacityVector<OutType> enumerate(VkResult (*func)(InType, uint32_t*, OutType*), InType inData) {
    uint32_t size = 0;
    VkResult result = func(inData, &size, nullptr);
    ASSERT_POSTCONDITION(result == VK_SUCCESS && size > 0, "enumerate-1 size error");
    utils::FixedCapacityVector<OutType> ret(size);
    result = func(inData, &size, ret.data());
    ASSERT_POSTCONDITION(result == VK_SUCCESS, "enumerate-1 error");
    return move(ret);
}

template <typename InTypeA, typename InTypeB, typename OutType>
utils::FixedCapacityVector<OutType> enumerate(
        VkResult (*func)(InTypeA, InTypeB, uint32_t*, OutType*), InTypeA inDataA, InTypeB inDataB) {
    uint32_t size = 0;
    VkResult result = func(inDataA, inDataB, &size, nullptr);
    ASSERT_POSTCONDITION(result == VK_SUCCESS && size > 0, "enumerate-2 size error");
    utils::FixedCapacityVector<OutType> ret(size);
    result = func(inDataA, inDataB, &size, ret.data());
    ASSERT_POSTCONDITION(result == VK_SUCCESS, "enumerate-2 error");
    return move(ret);
}


} // namespace filament::backend

bool operator<(const VkImageSubresourceRange& a, const VkImageSubresourceRange& b);

#endif // TNT_FILAMENT_BACKEND_VULKANUTILITY_H
