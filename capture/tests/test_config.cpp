#include <gtest/gtest.h>
#include <auralis/capture/config.hpp>
#include <nlohmann/json.hpp>

TEST(AudioConfigTest, DefaultValuesAreValid) {
    auralis::capture::AudioConfig cfg;
    EXPECT_NO_THROW(cfg.validate());
    EXPECT_EQ(cfg.sample_rate, 48000u);
    EXPECT_EQ(cfg.channels, 1u);
    EXPECT_EQ(cfg.buffer_size, 512u);
    EXPECT_FLOAT_EQ(cfg.vad_threshold, 0.01f);
}

TEST(AudioConfigTest, ValidationRejectsInvalidParams) {
    auralis::capture::AudioConfig cfg;
    cfg.sample_rate = 4000;
    EXPECT_THROW(cfg.validate(), std::invalid_argument);
    
    cfg.sample_rate = 48000;
    cfg.buffer_size = 100; // Non potenza di 2
    EXPECT_THROW(cfg.validate(), std::invalid_argument);
}

TEST(AudioConfigTest, JsonRoundtrip) {
    auralis::capture::AudioConfig original;
    original.sample_rate = 96000;
    original.channels = 2;
    original.format = "S32_LE";
    original.vad_threshold = 0.05f;

    nlohmann::json j = original.to_json();
    auto restored = auralis::capture::AudioConfig::from_json(j);

    EXPECT_EQ(original.sample_rate, restored.sample_rate);
    EXPECT_EQ(original.format, restored.format);
    EXPECT_FLOAT_EQ(original.vad_threshold, restored.vad_threshold);
}
