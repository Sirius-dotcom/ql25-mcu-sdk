# QL25 Platform 层使用指南

## 1. 文档目的

本文档面向业务层开发人员，说明 QL25 SDK 中 Platform 层的定位、目录结构、推荐调用方式和使用边界。

当前 Platform 层采用三层结构：

- `platform/utb_port`
- `platform/utb_osal`
- `platform/utb_hal`

推荐依赖关系如下：

```text
业务层 / 业务承载层
    ->
UTB Service / Adapter
    ->
utb_osal + utb_hal + utb_port
    ->
SDK osal + SDK drivers + SoC registers
```

业务代码应优先调用 `utb_*` 接口，不建议直接依赖 SoC 底层驱动头文件。

---

## 2. 目录说明

### 2.1 `platform/utb_port`

定位：项目公共基础能力层。

主要内容：

- 统一返回码
- 内存申请释放
- 打印输出
- 字节序宏
- 对外总入口头文件

核心头文件：

- `platform/utb_port/include/utb_port.h`
- `platform/utb_port/include/utb_api.h`

适合业务层直接使用的内容：

- `UTB_OK`
- `UTB_ERR_*`
- `UTB_MALLOC`
- `UTB_FREE`
- `UTB_PRINT`

---

### 2.2 `platform/utb_osal`

定位：对 SDK `platform/osal` 的项目级再封装。

主要内容：

- 线程
- 互斥锁
- 信号量
- 条件变量
- 软件定时器

核心头文件：

- `platform/utb_osal/include/utb_osal.h`
- `platform/utb_osal/include/utb_os_thread.h`
- `platform/utb_osal/include/utb_os_timer.h`

适用场景：

- 业务线程管理
- 模块同步与互斥
- 定时任务
- 模块间事件等待

---

### 2.3 `platform/utb_hal`

定位：设备访问封装层，用于隔离业务和底层驱动细节。

当前已提供：

- SPI
- I2C
- IRQ
- Flash 占位接口

核心头文件：

- `platform/utb_hal/include/utb_hal.h`
- `platform/utb_hal/include/utb_hal_spi.h`
- `platform/utb_hal/include/utb_hal_i2c.h`
- `platform/utb_hal/include/utb_hal_irq.h`
- `platform/utb_hal/include/utb_hal_flash.h`

说明：

- QL25 当前是 SoC，片内寄存器访问通常不需要走 I2C。
- 因此 `utb_hal_i2c` 当前不是业务主路径的必选接口。
- `utb_hal_flash` 目前只保留接口定义，返回 `UTB_ERR_UNSUPPORTED`，后续再按实际器件和访问路径补齐。

---

## 3. 推荐使用原则

### 3.1 业务层调 `utb_*`，不要直接调底层驱动

推荐：

- `utb_os_thread_create`
- `utb_os_sem_wait_ms`
- `utb_hal_spi_transfer`

不推荐：

- 业务代码直接 include `nuclei_sdk_soc.h`
- 业务代码直接调用 `QSPI_Init`
- 业务代码直接调用 `I2C_Init`
- 业务代码直接操作寄存器地址

原因：

- 避免业务代码与具体 SoC 驱动耦合
- 后续底层替换时改动面更小
- 业务层接口语义更统一

---

### 3.2 纯硬件驱动和 OS 能力要分层

建议：

- 线程、锁、同步、定时器，统一走 `utb_osal`
- 设备访问，统一走 `utb_hal`

不要把这两层混在一起，例如：

- 不要在设备访问函数里直接创建线程
- 不要在业务线程里直接改底层寄存器定义

---

### 3.3 业务层不要直接依赖 `platform/osal`

虽然 `utb_osal` 底下依赖 SDK `platform/osal`，但业务层应只依赖：

- `utb_port`
- `utb_osal`
- `utb_hal`

这样后续即使底层 `osal` 接口变更，业务层也不需要大范围修改。

---

## 4. 编译接入方式

如果业务应用需要使用 Platform 层，在应用 `Makefile` 中引入以下构建入口：

```make
include $(NUCLEI_SDK_ROOT)/platform/utb_port/build.mk
include $(NUCLEI_SDK_ROOT)/platform/utb_osal/build.mk
include $(NUCLEI_SDK_ROOT)/platform/utb_hal/build.mk
```

推荐接入顺序：

1. `utb_port`
2. `utb_osal`
3. `utb_hal`

完整示例可参考：

- `application/freertos/utb_osal_demo/Makefile`

---

## 5. 头文件使用建议

### 5.1 推荐包含方式

业务模块如果同时需要 Port、OSAL、HAL，可以直接包含总入口：

```c
#include "utb_api.h"
```

如果只需要其中一层，也可以按需包含：

```c
#include "utb_port.h"
#include "utb_osal.h"
#include "utb_hal_spi.h"
```

---

## 6. `utb_port` 使用说明

### 6.1 返回码

`utb_port.h` 统一定义了常见返回码：

- `UTB_OK`
- `UTB_ERR`
- `UTB_ERR_PARAM`
- `UTB_ERR_NOMEM`
- `UTB_ERR_TIMEOUT`
- `UTB_ERR_UNSUPPORTED`
- `UTB_ERR_BUSY`

推荐业务层统一返回这些错误码，而不是混用 `0/-1` 或底层驱动的私有错误码。

示例：

```c
int foo_init(void)
{
    if (bar_not_ready()) {
        return UTB_ERR_BUSY;
    }
    return UTB_OK;
}
```

---

### 6.2 内存和打印

接口：

- `UTB_MALLOC(sz)`
- `UTB_FREE(ptr)`
- `UTB_PRINT(...)`

说明：

- 当前 `UTB_MALLOC/UTB_FREE` 底层使用标准 `malloc/free`
- 当前 `UTB_PRINT` 底层使用 `printf`

建议：

- 业务公共代码统一用 `UTB_PRINT`
- 不要在业务代码里混用多个不同打印入口

---

## 7. `utb_osal` 使用说明

## 7.1 线程

主要接口：

- `utb_os_thread_create`
- `utb_os_thread_destroy`
- `utb_os_thread_sleep_ms`
- `utb_os_thread_stack_watermark`

线程创建参数结构体：

```c
typedef struct {
    char thread_name[32];
    uint32_t stack_words;
    uint32_t sched_priority;
    utb_thread_entry_t func_thread_call_back;
    void *arg;
} utb_os_thread_init_t;
```

最小示例：

```c
static void worker(void *arg)
{
    (void)arg;
    for (;;) {
        UTB_PRINT("worker alive\r\n");
        utb_os_thread_sleep_ms(1000);
    }
}

void app_start(void)
{
    utb_os_thread_init_t cfg = {0};

    strcpy(cfg.thread_name, "worker");
    cfg.stack_words = 512;
    cfg.sched_priority = 2;
    cfg.func_thread_call_back = worker;
    cfg.arg = NULL;

    (void)utb_os_thread_create(&cfg);
}
```

使用建议：

- `thread_name` 保持简短，便于调试
- `stack_words` 先保守给大，再通过 `utb_os_thread_stack_watermark` 收敛
- `sched_priority` 由模块统一规划，不要各业务模块随意定义

---

## 7.2 互斥锁

主要接口：

- `utb_os_mutex_create`
- `utb_os_mutex_destroy`
- `utb_os_mutex_lock`
- `utb_os_mutex_unlock`

适用场景：

- 共享状态保护
- 共享链表或缓冲区保护

示例：

```c
static utb_mutex_handle_t *g_lock;
static uint32_t g_counter;

void counter_inc(void)
{
    if (utb_os_mutex_lock(g_lock, OSAL_WAIT_FOREVER) == UTB_OK) {
        g_counter++;
        (void)utb_os_mutex_unlock(g_lock);
    }
}
```

注意：

- 加锁成功后必须保证解锁
- 不要长时间持锁做阻塞操作

---

## 7.3 信号量

主要接口：

- `utb_os_sem_create`
- `utb_os_sem_destroy`
- `utb_os_sem_wait_ms`
- `utb_os_sem_post`

适用场景：

- 等待后台任务完成
- 中断或线程事件通知
- 简单资源计数

示例：

```c
static utb_sem_handle_t *g_done_sem;

void producer_done(void)
{
    (void)utb_os_sem_post(g_done_sem);
}

int wait_done(void)
{
    return utb_os_sem_wait_ms(g_done_sem, 1000);
}
```

---

## 7.4 条件变量

主要接口：

- `utb_os_cond_create`
- `utb_os_cond_destroy`
- `utb_os_cond_wait`
- `utb_os_cond_signal`
- `utb_os_cond_broadcast`

适用场景：

- 等待某个共享状态变化
- 多线程初始化同步

典型模式：

```c
utb_os_mutex_lock(lock, OSAL_WAIT_FOREVER);
while (!ready) {
    utb_os_cond_wait(cond, lock);
}
utb_os_mutex_unlock(lock);
```

注意：

- `utb_os_cond_wait` 应与互斥锁配合使用
- 推荐使用 `while` 循环检查条件，而不是 `if`

---

## 7.5 软件定时器

主要接口：

- `utb_os_timer_create`
- `utb_os_timer_start`
- `utb_os_timer_stop`
- `utb_os_timer_reset`
- `utb_os_timer_delete`

配置结构：

```c
typedef struct {
    uint32_t delay_ms;
    uint32_t interval_ms;
    void *param;
    utb_timer_callback_t func_timer_callback;
} utb_os_timer_init_t;
```

说明：

- `interval_ms != 0` 时，创建周期定时器
- `interval_ms == 0` 且 `delay_ms != 0` 时，创建单次定时器

示例：

```c
static void timer_cb(void *arg)
{
    (void)arg;
    UTB_PRINT("timer fired\r\n");
}

void timer_start_demo(void)
{
    utb_os_timer_init_t cfg = {0};
    utb_os_timer_handle_t *tmr;

    cfg.interval_ms = 100;
    cfg.func_timer_callback = timer_cb;

    tmr = utb_os_timer_create(&cfg);
    if (tmr != NULL) {
        (void)utb_os_timer_start(tmr);
    }
}
```

注意：

- 回调函数应尽量短小
- 不要在定时器回调里做长耗时阻塞操作

---

## 8. `utb_hal` 使用说明

## 8.1 SPI

主要接口：

- `utb_hal_spi_open`
- `utb_hal_spi_close`
- `utb_hal_spi_set_clock_div`
- `utb_hal_spi_transfer`

适用场景：

- 外部 SPI 设备收发
- 简单控制命令和数据收发

示例：

```c
void spi_demo(void)
{
    utb_hal_spi_cfg_t cfg = {0};
    utb_hal_spi_handle_t *spi;
    uint8_t tx[2] = {0x9F, 0x00};
    uint8_t rx[2] = {0};

    cfg.bus = UTB_HAL_SPI_BUS_QSPI1;
    cfg.master = true;
    cfg.mode = 0;
    cfg.bits_per_word = 8;
    cfg.clock_div = 8;

    spi = utb_hal_spi_open(&cfg);
    if (spi == NULL) {
        return;
    }

    (void)utb_hal_spi_transfer(spi, tx, rx, sizeof(tx));
    utb_hal_spi_close(spi);
}
```

说明：

- QL25 当前外部 SPI 口对应 `QSPI1/2/3`
- 业务代码只看 `UTB_HAL_SPI_BUS_QSPI1/2/3`
- 不需要直接调用 `QSPI_Init`

---

## 8.2 I2C

主要接口：

- `utb_hal_i2c_open`
- `utb_hal_i2c_close`
- `utb_hal_i2c_mem_write`
- `utb_hal_i2c_mem_read`

说明：

- 该接口保留给外挂 I2C 设备场景
- 对于 QL25 片内寄存器访问，通常不需要走 I2C

示例：

```c
void i2c_demo(void)
{
    utb_hal_i2c_cfg_t cfg = {0};
    utb_hal_i2c_handle_t *i2c;
    uint8_t val = 0;

    cfg.bus = UTB_HAL_I2C_BUS_0;
    cfg.speed_hz = 100000;

    i2c = utb_hal_i2c_open(&cfg);
    if (i2c == NULL) {
        return;
    }

    (void)utb_hal_i2c_mem_read(i2c, 0x50, 0x10, 1, &val, 1);
    utb_hal_i2c_close(i2c);
}
```

---

## 8.3 IRQ

主要接口：

- `utb_hal_irq_register`
- `utb_hal_irq_enable`
- `utb_hal_irq_disable`

适用场景：

- 设备中断注册
- 中断使能和关闭

说明：

- 业务代码如果确实需要注册裸中断，可通过此接口统一接入
- 更推荐由设备适配层统一管理中断，再向业务线程发事件

---

## 8.4 Flash

当前接口状态：

- 已定义
- 尚未落地具体实现
- 目前返回 `UTB_ERR_UNSUPPORTED`

因此业务层当前不要把 `utb_hal_flash` 当成可用接口。

---

## 9. 业务开发推荐写法

推荐模块写法如下：

1. 模块初始化阶段创建线程、锁、信号量和定时器
2. 线程函数只通过 `utb_osal` 做同步与调度
3. 设备访问只通过 `utb_hal`
4. 日志和返回码统一用 `utb_port`

推荐示意：

```text
biz_xxx.c
  -> utb_os_thread_create
  -> utb_os_sem_wait_ms
  -> utb_hal_spi_transfer
  -> UTB_PRINT
```

不推荐示意：

```text
biz_xxx.c
  -> xTaskCreate
  -> QSPI_Init
  -> 直接寄存器写地址
  -> printf / printk / 其他混用
```

---

## 10. 当前可参考示例

推荐参考：

- `application/freertos/utb_osal_demo/main.c`

该示例已经覆盖：

- 线程创建
- 互斥锁
- 信号量
- 条件变量
- 软件定时器
- 栈水位查询

适合作为业务层接入 Platform 层的起始模板。

---

## 11. 当前限制说明

当前 Platform 层还处于第一版收敛阶段，使用时请注意：

- `utb_osal` 已完成接口收敛，但还需要结合实际应用继续验证
- `utb_hal_flash` 目前不可用
- `utb_hal_i2c` 保留为外挂器件接口，不是 QL25 片内寄存器主访问路径
- 若业务需要更复杂能力，例如事件组、消息邮箱、DMA 异步收发，需要在现有层次上继续补接口，不建议业务直接绕开 Platform 层

---

## 12. 总结

对业务层开发人员来说，可以把 Platform 层简单理解为：

- `utb_port`：公共基础能力
- `utb_osal`：线程和同步能力
- `utb_hal`：设备访问能力

推荐原则只有一句：

**业务代码优先依赖 `utb_*`，不要直接依赖底层 SDK 驱动和寄存器定义。**

