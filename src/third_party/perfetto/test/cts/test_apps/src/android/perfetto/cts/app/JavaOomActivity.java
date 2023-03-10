/*
 * Copyright (C) 2023 The Android Open Source Project
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

package android.perfetto.cts.app;

import android.app.Activity;
import android.os.Bundle;

import java.util.ArrayList;

public class JavaOomActivity extends Activity {
    @Override
    public void onCreate(Bundle state) {
        super.onCreate(state);

        new Thread(new Runnable() {
            public void run() {
                try {
                    runAllocationLoop();
                } catch (Exception ex) {
                    ex.printStackTrace();
                }
            }
        }).start();
    }

    private static void runAllocationLoop() {
        ArrayList<byte[]> leaky = new ArrayList<>();
        try {
            for (;;) {
                leaky.add(new byte[1024 * 1024]);
                try {
                    Thread.sleep(10);
                } catch (InterruptedException ignored) {
                }
            }
        } catch (OutOfMemoryError e) {
          leaky.clear();
        }
    }
}
