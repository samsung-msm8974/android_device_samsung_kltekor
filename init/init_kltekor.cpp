/*
   Copyright (c) 2020, The LineageOS Project. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/strings.h>

#include "property_service.h"

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

using android::base::GetProperty;
using android::base::ReadFileToString;
using android::base::Trim;

// copied from build/tools/releasetools/ota_from_target_files.py
// but with "." at the end and empty entry
std::vector<std::string> ro_product_props_default_source_order = {
    "",
    "product.",
    "product_services.",
    "odm.",
    "vendor.",
    "system.",
    "system_ext.",    
};

void property_override(char const prop[], char const value[], bool add = true)
{
    auto pi = (prop_info *) __system_property_find(prop);

    if (pi != nullptr) {
        __system_property_update(pi, value, strlen(value));
    } else if (add) {
        __system_property_add(prop, strlen(prop), value, strlen(value));
    }
}

void vendor_load_properties()
{
    const std::string bootloader = GetProperty("ro.bootloader", "");

    const auto set_ro_product_prop = [](const std::string &source,
            const std::string &prop, const std::string &value) {
        auto prop_name = "ro.product." + source + prop;
        property_override(prop_name.c_str(), value.c_str(), false);
    };

    if (bootloader.find("G900K") == 0) {
        /* kltektt - KT Corp (formerly Korea Telecom) */
        for (const auto &source : ro_product_props_default_source_order) {
            set_ro_product_prop(source, "build.fingerprint", "samsung/kltektt/kltektt:6.0.1/MMB29M/G900KKTU1CPL5:user/release-keys");
            set_ro_product_prop(source, "device", "kltektt");
            set_ro_product_prop(source, "model", "SM-G900K");
            set_ro_product_prop(source, "name", "kltektt");
        }
        property_override("ro.build.description", "kltektt-user 6.0.1 MMB29M G900KKTU1CPL5 release-keys");
        property_override("ro.build.product", "kltektt");
    } else if (bootloader.find("G900L") == 0) {
        /* kltelgt - LG Uplus */
        for (const auto &source : ro_product_props_default_source_order) {
            set_ro_product_prop(source, "build.fingerprint", "samsung/kltelgt/kltelgt:6.0.1/MMB29M/G900LKLU1CPL5:user/release-keys");
            set_ro_product_prop(source, "device", "kltelgt");
            set_ro_product_prop(source, "model", "SM-G900L");
            set_ro_product_prop(source, "name", "kltelgt");
        }
        property_override("ro.build.description", "kltelgt-user 6.0.1 MMB29M G900LKLU1CPL5 release-keys");
        property_override("ro.build.product", "kltelgt");
    } else if (bootloader.find("G900S") == 0) {
        /* klteskt - SK Telecom */
        for (const auto &source : ro_product_props_default_source_order) {
            set_ro_product_prop(source, "build.fingerprint", "samsung/klteskt/klteskt:6.0.1/MMB29M/G900SKSU1CPL5:user/release-keyss");
            set_ro_product_prop(source, "device", "klteskt");
            set_ro_product_prop(source, "model", "SM-G900S");
            set_ro_product_prop(source, "name", "klteskt");
        }
        property_override("ro.build.description", "klteskt-user 6.0.1 MMB29M G900SKSU1CPL5 release-keys");
        property_override("ro.build.product", "klteskt");
    }

    const std::string device = GetProperty("ro.product.device", "");
    LOG(INFO) << "Found bootloader " << bootloader << ". " << "Setting build properties for " << device << ".\n";
}
